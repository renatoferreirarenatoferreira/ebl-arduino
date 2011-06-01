using System;
using System.Collections;
using System.Diagnostics;
using System.Text;

namespace ebl_arduino.examples
{
    /// <summary>
    /// Client application for LCDWriter sketch.
    /// </summary>
    class LCDWrite
    {
        //types of parameters
        private const int PARAMETER_BACKLIGHT = 0;
        private const int PARAMETER_PORT = 1;
        private const int PARAMETER_TEXT = 2;

        //types of messages
        private const byte BACKLIGHT_ON_MESSAGE = 0;
        private const byte BACKLIGHT_OFF_MESSAGE = 1;
        private const byte TEXT_MESSAGE = 2;

        /// <summary>
        /// Applications entry point.
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Hashtable Arguments = ArgumentParse(args);

            if (Arguments.ContainsKey(PARAMETER_PORT) &&
                (Arguments.ContainsKey(PARAMETER_BACKLIGHT) || Arguments.ContainsKey(PARAMETER_TEXT)))
            {
                Console.Write("Port: ");
                Console.WriteLine(Arguments[PARAMETER_PORT]);
                if (Arguments.ContainsKey(PARAMETER_BACKLIGHT))
                {
                    Console.Write("Backlight: ");
                    if ((bool)Arguments[PARAMETER_BACKLIGHT])
                        Console.WriteLine("On");
                    else
                        Console.WriteLine("Off");
                }
                if (Arguments.ContainsKey(PARAMETER_TEXT))
                {
                    Console.Write("Message: '");
                    Console.Write(Arguments[PARAMETER_TEXT]);
                    Console.WriteLine("'");
                }

                try
                {
                    //create object
                    AdvancedSerialClient ArduinoClient = new AdvancedSerialClient();
                    //connect to device
                    ArduinoClient.Open(Arguments[PARAMETER_PORT].ToString(), 115200);

                    //command for LED
                    if (Arguments.ContainsKey(PARAMETER_BACKLIGHT))
                    {
                        SerialProtocol.AdvancedSerialMessage BacklightMessage = new SerialProtocol.AdvancedSerialMessage();

                        if ((bool)Arguments[PARAMETER_BACKLIGHT])
                            BacklightMessage.ID = BACKLIGHT_ON_MESSAGE;
                        else
                            BacklightMessage.ID = BACKLIGHT_OFF_MESSAGE;

                        ArduinoClient.Send(BacklightMessage);
                    }

                    //command for text
                    if (Arguments.ContainsKey(PARAMETER_TEXT))
                    {
                        SerialProtocol.AdvancedSerialMessage TextMessage = new SerialProtocol.AdvancedSerialMessage();
                        TextMessage.ID = TEXT_MESSAGE;
                        TextMessage.Payload = new System.Text.ASCIIEncoding().GetBytes(Arguments[PARAMETER_TEXT].ToString());
                        TextMessage.Size = (byte)TextMessage.Payload.Length;
                        ArduinoClient.Send(TextMessage);
                    }
                }
                catch (Exception ex)
                {
                    Console.Write("Error: " + ex.Message);
                }
            }
            else
            {
                Console.WriteLine("Invalid parameters!");
                Console.WriteLine();
                Console.WriteLine("Write a message to LCD display. It's a serial client for LCDWriter sketch from abl-arduino lirary.");
                Console.WriteLine();
                Console.WriteLine("usage: " + Process.GetCurrentProcess().ProcessName + " /P port [/On | /Off] [message]");
                Console.WriteLine();
                Console.WriteLine("   message   Message to be written on LCD display (16x2).");
                Console.WriteLine();
                Console.WriteLine("   /P        Serial port.");
                Console.WriteLine("   /On       Turn backlight led on.");
                Console.WriteLine("   /Off      Turn backlight led off.");
            }
        }

        /// <summary>
        /// Parse arguments.
        /// </summary>
        /// <param name="args">Argument array from Main() method.</param>
        /// <returns>Hashtable of parsed arguments.</returns>
        private static Hashtable ArgumentParse(string[] args)
        {
            Hashtable Return = new Hashtable();

            string Key;
            bool ReadPort = false;
            bool ResetMessage = false;
            foreach (String arg in args)
            {
                if (arg.StartsWith("-") || arg.StartsWith("/"))
                {
                    Key = arg.Substring(1, arg.Length - 1).ToLower();

                    if (Key.Equals("on"))
                    {
                        if (!Return.ContainsKey(PARAMETER_BACKLIGHT))
                            Return.Add(PARAMETER_BACKLIGHT, true);
                    }
                    else if (Key.Equals("off"))
                    {
                        if (!Return.ContainsKey(PARAMETER_BACKLIGHT))
                            Return.Add(PARAMETER_BACKLIGHT, false);
                    }
                    else if (Key.StartsWith("p"))
                    {
                        if (!Return.ContainsKey(PARAMETER_PORT))
                            ReadPort = true;
                    }

                    ResetMessage = true;
                }
                else if (ReadPort)
                {
                    Return.Add(PARAMETER_PORT, arg);
                    ReadPort = false;
                }
                else
                {
                    if (ResetMessage)
                    {
                        if (Return.ContainsKey(PARAMETER_TEXT))
                            Return.Remove(PARAMETER_TEXT);

                        ResetMessage = false;
                    }

                    if (!Return.ContainsKey(PARAMETER_TEXT))
                    {
                        Return.Add(PARAMETER_TEXT, new StringBuilder(arg));
                    }
                    else
                    {
                        ((StringBuilder)Return[PARAMETER_TEXT]).Append(" ").Append(arg);
                    }
                }
            }

            return Return;
        }
    }
}
