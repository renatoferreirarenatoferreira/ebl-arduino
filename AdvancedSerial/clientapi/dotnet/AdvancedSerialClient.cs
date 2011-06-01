using System;
using System.IO.Ports;

namespace ebl_arduino
{
    /// <summary>
    /// Serial client for AdvancedSerial.
    /// </summary>
    public class AdvancedSerialClient : SerialProtocol
    {
        /// <summary>
        /// Serial port.
        /// </summary>
        private SerialPort Port;

        /// <summary>
        /// Default constructor.
        /// </summary>
        public AdvancedSerialClient()
        {

        }

        /// <summary>
        /// Verify transport connection.
        /// </summary>
        /// <returns>Return true if connected.</returns>
        public override bool IsConnected()
        {
            if (this.Port != null)
                return this.Port.IsOpen;
            else
                return false;
        }

        /// <summary>
        /// Close serial port.
        /// </summary>
        public override void Close()
        {
            this.ConnectionStream.Close();
            this.Port.Close();
        }

        /// <summary>
        /// Starts device communication through a serial port.
        /// </summary>
        /// <param name="Port">Port name.</param>
        /// <param name="BaudRate">Port baud rate.</param>
        public void Open(String Port, int BaudRate)
        {
            if (this.Port != null && this.Port.IsOpen)
                this.Port.Close();

            this.Port = new SerialPort(Port);
            this.Port.BaudRate = BaudRate;
            this.Port.Open();
            this.ConnectionStream = this.Port.BaseStream;
            this.Receive();

            this.Flush();

            this.Send(MessageTypes.DISCOVERY_REQUEST);
        }
    }
}
