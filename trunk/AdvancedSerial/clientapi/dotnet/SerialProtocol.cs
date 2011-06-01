using System;
using System.IO;
using System.Threading;

namespace ebl_arduino
{
    /// <summary>
    /// Protocol implementation for AdvancedSerial.
    /// </summary>
    public abstract class SerialProtocol
    {
        /// <summary>
        /// Type of AdvancedSerial messages.
        /// </summary>
        public enum MessageTypes
        {
            /// <summary>
            /// Common message.
            /// </summary>
            DEBUG = 0x01,

            /// <summary>
            /// Common message.
            /// </summary>
            MESSAGE = 0x02,

            /// <summary>
            /// Message acknowledge.
            /// </summary>
            MESSAGE_ACKNOWLEDGE = 0x03,

            /// <summary>
            /// Discovery request message.
            /// </summary>
            DISCOVERY_REQUEST = 0x04,

            /// <summary>
            /// Discovery response message.
            /// </summary>
            DISCOVERY_RESPONSE = 0x05
        }

        /// <summary>
        /// Message class.
        /// </summary>
        public class AdvancedSerialMessage
        {
            /// <summary>
            /// Type of message.
            /// </summary>
            public MessageTypes Type = MessageTypes.MESSAGE;

            /// <summary>
            /// Message ID.
            /// </summary>
            public byte ID;

            /// <summary>
            /// Payload size.
            /// </summary>
            public byte Size = 0;

            /// <summary>
            /// Message payload.
            /// </summary>
            public byte[] Payload;
        }

        /// <summary>
        /// Connection states.
        /// </summary>
        public enum ConnectionState
        {
            /// <summary>
            /// Receiving start of text.
            /// </summary>
            ReadingSTX,

            /// <summary>
            /// Receiving end of text.
            /// </summary>
            ReadingETX,

            /// <summary>
            /// Receiving header.
            /// </summary>
            ReadingHeader,

            /// <summary>
            /// Receiving payload.
            /// </summary>
            ReadingPayload
        }

        /// <summary>
        /// Size of message header.
        /// </summary>
        public const int MESSAGE_HEADER_SIZE = 3;

        /// <summary>
        /// Size of message delimiter.
        /// </summary>
        public const int MESSAGE_DELIMITER_SIZE = 1;

        /// <summary>
        /// Maximum size of message payload.
        /// </summary>
        public const int MESSAGE_MAX_PAYLOAD_SIZE = 32;

        /// <summary>
        /// Maximum size of message.
        /// </summary>
        public const int MESSAGE_SIZE = MESSAGE_MAX_PAYLOAD_SIZE + MESSAGE_HEADER_SIZE;

        /// <summary>
        /// Start of message.
        /// </summary>
        public const byte STX = 0x02;

        /// <summary>
        /// End of message.
        /// </summary>
        public const byte ETX = 0x03;

        /// <summary>
        /// Delegate method for MessageReceived.
        /// </summary>
        public delegate void MessageReceivedCallback(AdvancedSerialMessage Message);

        /// <summary>
        /// Occurs when the a message is received.
        /// </summary>
        public event MessageReceivedCallback MessageReceived;

        /// <summary>
        /// Protocol stream.
        /// </summary>
        protected Stream ConnectionStream;

        /// <summary>
        /// Verify transport connection.
        /// </summary>
        /// <returns>Return true if connected.</returns>
        public abstract bool IsConnected();

        /// <summary>
        /// Close transport connection.
        /// </summary>
        public abstract void Close();

        /// <summary>
        /// Timeout for message confirmation.
        /// </summary>
        public int MessageTimeout = 2000;

        private AdvancedSerialMessage InputMessage = new AdvancedSerialMessage();
        private byte[] InputBuffer = new byte[MESSAGE_SIZE];
        private byte[] InputDelimiter = new byte[1];
        private ConnectionState State = ConnectionState.ReadingSTX;
        private int ReceivedBytes;
        private int RequestedBytes;

        /// <summary>
        /// Send message.
        /// </summary>
        /// <param name="Message">Message to send.</param>
        public void Send(AdvancedSerialMessage Message)
        {
            this.Send(Message.Type, Message.ID, Message.Size, Message.Payload);
        }

        /// <summary>
        /// Send message.
        /// </summary>
        /// <param name="Type">Message type.</param>
        /// <param name="ID">Message ID.</param>
        /// <param name="Size">Payload size.</param>
        /// <param name="Payload">Message payload</param>
        public void Send(MessageTypes Type, byte ID, byte Size, byte[] Payload)
        {
            if (Size >= 0 && Size <= MESSAGE_MAX_PAYLOAD_SIZE)
            {
                this.ConnectionStream.WriteByte(STX);
                this.ConnectionStream.WriteByte((byte)Type);
                this.ConnectionStream.WriteByte(ID);
                this.ConnectionStream.WriteByte(Size);
                if (Size > 0) this.ConnectionStream.Write(Payload, 0, Size);
                this.ConnectionStream.WriteByte(ETX);

                //wait for confirmations
                if (Type == MessageTypes.MESSAGE ||
                    Type == MessageTypes.DISCOVERY_REQUEST)
                {
                    lock (this.ConnectionStream)
                    {
                        if (!Monitor.Wait(this.ConnectionStream, this.MessageTimeout))
                            throw new TimeoutException("Device not responding.");
                    }
                }
            }
            else
            {
                throw new TimeoutException("Accepted message's payload size between 0 and " + MESSAGE_MAX_PAYLOAD_SIZE + ".");
            }
        }

        /// <summary>
        /// Send simple message without payload.
        /// </summary>
        /// <param name="Type">Message type.</param>
        public void Send(MessageTypes Type)
        {
            this.Send(Type, 0, 0, null);
        }

        /// <summary>
        /// Clear communication stream.
        /// </summary>
        public void Flush()
        {
            byte[] flushdata = new byte[MESSAGE_DELIMITER_SIZE+MESSAGE_SIZE+MESSAGE_DELIMITER_SIZE];
            for (int i = 0; i < flushdata.Length; i++)
            {
                flushdata[i] = (byte)0x00;
            }
            this.ConnectionStream.Write(flushdata, 0, flushdata.Length);
            this.ConnectionStream.Flush();
        }

        /// <summary>
        /// Receive next amount of data.
        /// </summary>
        protected void Receive()
        {
            this.ReceivedBytes = 0;

            if (this.DataReceivingCallback == null)
                this.DataReceivingCallback = new AsyncCallback(DataReceiving);

            try
            {
                switch (this.State)
                {
                    case ConnectionState.ReadingSTX:
                    case ConnectionState.ReadingETX:
                        this.ConnectionStream.BeginRead(this.InputDelimiter, 0, MESSAGE_DELIMITER_SIZE, this.DataReceivingCallback, null);
                        this.RequestedBytes = MESSAGE_DELIMITER_SIZE;
                        break;

                    case ConnectionState.ReadingHeader:
                        this.ConnectionStream.BeginRead(this.InputBuffer, 0, MESSAGE_HEADER_SIZE, this.DataReceivingCallback, null);
                        this.RequestedBytes = MESSAGE_HEADER_SIZE;
                        break;

                    case ConnectionState.ReadingPayload:
                        this.ConnectionStream.BeginRead(this.InputBuffer, 0, this.InputMessage.Size, this.DataReceivingCallback, null);
                        this.RequestedBytes = this.InputMessage.Size;
                        break;
                }
            }
            catch (Exception)
            {
                this.Close();
            }
        }

        private AsyncCallback DataReceivingCallback;
        private void DataReceiving(IAsyncResult ar)
        {
            int ReceivedSize = 0;
            try
            {
                ReceivedSize = this.ConnectionStream.EndRead(ar);
            }
            catch (Exception)
            {
                this.Close();
                return;
            }

            //verify if buffer was completelly received
            this.ReceivedBytes += ReceivedSize;
            if (this.ReceivedBytes < this.RequestedBytes)
            {
                try
                {
                    this.ConnectionStream.BeginRead(this.InputBuffer, ReceivedBytes, RequestedBytes - ReceivedBytes, this.DataReceivingCallback, null);
                }
                catch (Exception)
                {
                    this.Close();
                }
                return;
            }

            switch (this.State)
            {
                case ConnectionState.ReadingSTX:
                    if (this.InputDelimiter[0] == STX)
                    {
                        this.State = ConnectionState.ReadingHeader;
                    }
                    break;

                case ConnectionState.ReadingHeader:
                    this.InputMessage.Type = (MessageTypes)this.InputBuffer[0];
                    this.InputMessage.Size = this.InputBuffer[1];
                    if (this.InputMessage.Size == 0)
                    {
                        //payload is empty
                        this.State = ConnectionState.ReadingETX;
                    }
                    else if (this.InputMessage.Size > 0 && this.InputMessage.Size <= MESSAGE_MAX_PAYLOAD_SIZE)
                    {
                        //read
                        this.State = ConnectionState.ReadingPayload;
                    }
                    else
                    {
                        //avoid wrong sized messages
                        this.State = ConnectionState.ReadingSTX;
                    }
                    break;

                case ConnectionState.ReadingPayload:
                    Array.Copy(this.InputBuffer, this.InputMessage.Payload, this.InputMessage.Size);
                    this.State = ConnectionState.ReadingETX;
                    break;

                case ConnectionState.ReadingETX:
                    if (this.InputDelimiter[0] == ETX)
                    {
                        if (this.InputMessage.Type == MessageTypes.DISCOVERY_RESPONSE ||
                            this.InputMessage.Type == MessageTypes.MESSAGE_ACKNOWLEDGE)
                        {
                            lock (this.ConnectionStream)
                            {
                                Monitor.Pulse(this.ConnectionStream);
                            }
                        }
                        else if (this.InputMessage.Type == MessageTypes.MESSAGE)
                        {
                            this.Send(MessageTypes.MESSAGE_ACKNOWLEDGE);
                            this.MessageReceived(this.InputMessage);
                        }
                        else if (this.InputMessage.Type == MessageTypes.DISCOVERY_REQUEST)
                        {
                            this.Send(MessageTypes.DISCOVERY_RESPONSE);
                        }
                    }

                    this.State = ConnectionState.ReadingSTX;
                    break;
            }

            this.Receive();
        }
    }
}
