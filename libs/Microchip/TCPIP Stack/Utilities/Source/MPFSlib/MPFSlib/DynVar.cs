﻿/*********************************************************************
 *
 *    Dynamic Variable Parser Library
 *
 *********************************************************************
 * FileName:        DynVar.cs
 * Dependencies:    Microsoft .NET Framework 2.0
 * Processor:       x86
 * Complier:        Microsoft Visual C# 2008 Express Edition
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Microchip Technology Inc. ("Microchip") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2008 Microchip
 * Technology Inc.  All rights reserved.
 *
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date   		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Elliott Wood		4/17/2008	    Original
 ********************************************************************/
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace Microchip
{

    public class DynamicVariableParser
    {
        UInt32 offSetCounter = 0;
		UInt32 parseItrtn=0;
		UInt32 tempFileRcrdLen=0;
		
        #region Fields
        private List<DynamicVariable> vars;
        private Regex parser = new Regex(@"~(inc:[A-Za-z0-9\ \.-_\\/]{1,60}|[A-Za-z0-9_]{0,40}(\([A-Za-z0-9_,\ ]*\))?)~");
        private ASCIIEncoding ascii = new ASCIIEncoding();
        private String projectDir;
        #endregion

        #region String Constants
        const string HTTPPRINT_H_HEADER = 
            "/**************************************************************\r\n" +
            " * HTTPPrint.h\r\n" +
            " * Provides callback headers and resolution for user\'s custom\r\n" +
            " * HTTP Application.\r\n" +
            " * \r\n" +
            " * This file is automatically generated by the MPFS Utility\r\n" +
            " * ALL MODIFICATIONS WILL BE OVERWRITTEN BY THE MPFS GENERATOR\r\n" +
            " **************************************************************/\r\n\r\n" +
            "#ifndef __HTTPPRINT_H\r\n" +
            "#define __HTTPPRINT_H\r\n\r\n" +
            "#include \"TCPIP Stack/TCPIP.h\"\r\n\r\n" +
            "#if defined(STACK_USE_HTTP2_SERVER)\r\n\r\n" +
            "extern HTTP_STUB httpStubs[MAX_HTTP_CONNECTIONS];\r\n" +
            "extern BYTE curHTTPID;\r\n\r\n" +
            "void HTTPPrint(DWORD callbackID);\r\n";
        const string HTTPPRINT_H_MIDDLE =
            "\r\nvoid HTTPPrint(DWORD callbackID)\r\n" +
            "{\r\n" +
            "	switch(callbackID)\r\n" +
            "	{\r\n";
        const string HTTPPRINT_H_FOOTER =
            "		default:\r\n" +
            "			// Output notification for undefined values\r\n" +
            "			TCPPutROMArray(sktHTTP, (ROM BYTE*)\"!DEF\", 4);\r\n" +
            "	}\r\n\r\n" +
            "	return;\r\n" +
            "}\r\n\r\n" +
            "void HTTPPrint_(void)\r\n" +
            "{\r\n" +
            "	TCPPut(sktHTTP, '~');\r\n" +
            "	return;\r\n" +
            "}\r\n\r\n#endif\r\n\r\n#endif\r\n";
        #endregion

        #region Constructor
        public DynamicVariableParser(String path)
        {
            this.projectDir = path;
            vars = new List<DynamicVariable>();
            
            // Read previous index file if it exists.
            try
            {
                if (File.Exists(projectDir + "HTTPPrint.idx"))
                {
                    StreamReader fin = new StreamReader(projectDir + "HTTPPrint.idx");
                    String s = fin.ReadLine();

                    // Ignore old MPFS2.0 HTTPPrint.idx files
                    if (!s.Contains("|"))
                    {
                        while (s != null)
                        {
                            DynamicVariable dv = new DynamicVariable(s);
                            vars.Add(dv);
                            s = fin.ReadLine();
                        }
                    }
                    fin.Close();
                }
            }
            catch
            {
                // do nothing...just won't have old index information
            }
        }
        #endregion

        /// <summary>
        /// Parses and indexes a file for dynamic variables
        /// </summary>
        /// <param name="file">The MPFSFileRecord to parse</param>
        /// <returns>An MPFSFileRecord of indexes, or null if no variables were found</returns>
        public MPFSFileRecord Parse(MPFSFileRecord file)
        {
            byte[] idxData = new byte[0];
			UInt32 dynVarCntr=0;
					
      	
            MatchCollection matches = parser.Matches(ascii.GetString(file.data));
            foreach(Match m in matches)
            {
                int i = GetIndex(m.Value.Replace(" ","").Replace("~",""));
				Array.Resize(ref idxData, idxData.Length + 8);                
                idxData[idxData.Length - 8] = (byte)m.Index;
                idxData[idxData.Length - 7] = (byte)(m.Index >> 8);
                idxData[idxData.Length - 6] = (byte)(m.Index >> 16);
                idxData[idxData.Length - 5] = (byte)(m.Index >> 24);
                idxData[idxData.Length - 4] = (byte)i;
                idxData[idxData.Length - 3] = (byte)(i >> 8);
                idxData[idxData.Length - 2] = (byte)(i >> 16);
                idxData[idxData.Length - 1] = (byte)(i >> 24);


				Array.Resize(ref file.dynVarOffsetAndIndexID, file.dynVarOffsetAndIndexID.Length + 8);

				file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 1] = (byte)(i >> 24);
   			    file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 2] = (byte)(i >> 16);
   			    file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 3] = (byte)(i >> 8);
   			    file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 4] = ((byte)i);


				file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 5] = (byte)(m.Index >> 24);
				file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 6] = (byte)(m.Index >> 16);
				file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 7] = (byte)(m.Index >> 8);
				file.dynVarOffsetAndIndexID[file.dynVarOffsetAndIndexID.Length - 8] = (byte)m.Index;

				file.dynVarCntr = ++dynVarCntr;
                offSetCounter = offSetCounter + 8;
			
            }
			
			if(parseItrtn == (UInt32)0x0)
			{
				file.fileRecordOffset = (UInt32)0x0;
				offSetCounter = (UInt32)0x0;
			}
			else
			{
				//file.fileRecordOffset=tempFileRcrdLen+1;
				file.fileRecordOffset=tempFileRcrdLen;
			}
			
			file.fileRecordLength = 4 /* 4 bytes for file record length itself*/
									+2 /*To store the hasIndex/isZipped flag*/
									//+(UInt32)file.FileName.Length 
									+ file.dynVarCntr*8;
			
			tempFileRcrdLen += file.fileRecordLength;

			parseItrtn++;
				

			// Determine if any matches were made
            if (idxData.Length == 0)
                return null;
            else
            {
                // Set up new file record
                MPFSFileRecord idxFile = new MPFSFileRecord();
                idxFile.FileName = "";
                idxFile.fileDate = file.fileDate;
                idxFile.isIndex = true;
                idxFile.data = idxData;
                return idxFile;
            }

        }

        /// <summary>
        /// Writes out HTTPPrint.h and HTTPPrint.idx if necessary
        /// </summary>
        /// <returns>TRUE if the files were written, FALSE if no changes are needed</returns>
        public bool WriteIndices()
        {
            // Determine if an update is necessary
            bool isChanged = false;
            foreach (DynamicVariable dv in vars)
            {
                if ((dv.WasUsed && dv.Count == 0) ||
                    (!dv.WasUsed && dv.Count != 0))
                {
                    isChanged = true;
                    break;
                }
            }
            if (!isChanged)
                return false;

            // Write out HTTPPrint.idx
            StreamWriter fout = new StreamWriter(projectDir + "HTTPPrint.idx", false);
            foreach (DynamicVariable dv in vars)
            {
                if (dv.Count > 0)
                    fout.Write('+');
                fout.WriteLine(dv.Name);
            }
            fout.Flush();
            fout.Close();

            // Begin writing HTTPPrint.h
            fout = new StreamWriter(projectDir + "HTTPPrint.h", false);
            fout.Write(HTTPPRINT_H_HEADER);

            // Write the prototypes
            List<string> prototypes = new List<string>();
            Regex rGetParts = new Regex(@"([A-Za-z0-9_]{0,40})(\(([A-Za-z0-9_,]*)\))?");
            foreach (DynamicVariable dv in vars)
            {
                if (dv.Name.StartsWith("inc:") || dv.Count == 0)
                    continue;
                Match m = rGetParts.Match(dv.Name);
                if (prototypes.Contains(m.Groups[1].Value))
                    continue;

                // Add the prototype
                prototypes.Add(m.Groups[1].Value);
                fout.Write("void HTTPPrint_" + m.Groups[1].Value + "(");
                if(m.Groups.Count == 4 && m.Groups[3].Value.Length > 0)
                {
                    int numParams = m.Groups[3].Value.Split(',').Length;
                    for (int i = 0; i < numParams - 1; i++)
                        fout.Write("WORD,");
                    fout.Write("WORD");
                }
                else
                {
                    fout.Write("void");
                }
                fout.Write(");\r\n");
            }

            // Write the function itself
            fout.Write(HTTPPRINT_H_MIDDLE);
            int index = 0;
            foreach (DynamicVariable dv in vars)
            {
                if (dv.Count == 0)
                {
                    index++;
                    continue;
                }

                fout.Write("        case 0x" + Convert.ToString(index++, 16).PadLeft(8, '0') + ":\r\n");

                // Write the actual case statement
                if(dv.Name.StartsWith("inc:"))
                {
                    fout.Write("\t\t\tHTTPIncFile((ROM BYTE*)\"" + dv.Name.Substring(4) + "\");\r\n\t\t\tbreak;\r\n");
                }
                else
                {
                    fout.Write("\t\t\tHTTPPrint_" + dv.Name);
                    if(!dv.Name.EndsWith(")"))
                        fout.Write("()");
                    fout.Write(";\r\n\t\t\tbreak;\r\n");
                }
            }

            // Write the footer part
            fout.Write(HTTPPRINT_H_FOOTER);
            fout.Flush();
            fout.Close();
            
            return true;
        }

        #region Private Methods
        /// <summary>
        /// Finds the index of a dynamic variable, or creates a new one
        /// </summary>
        /// <param name="name"></param>
        /// <returns>The index of the dynamic variable</returns>
        private int GetIndex(String name)
        {
            // Search for the dynamic variable
            DynamicVariable dv = new DynamicVariable(name);
            int i = vars.IndexOf(dv);
            
            // If not found, add a new one
            if (i == -1)
            {
                vars.Add(dv);
                i = vars.Count - 1;
            }

            // Mark as used and return the index
            vars[i].Count++;
            return i;
        }
        #endregion

    }

    public class DynamicVariable
    {
        #region Fields
        private String name;
        private bool wasUsed;
        private int count;
        #endregion

        #region Properties
        /// <summary>
        /// Gets or sets the name of this DynamicVariable
        /// </summary>
        public String Name
        {
            get { return this.name; }
            set { this.name = value; }
        }

       /// <summary>
        /// Indicates if this specific instance was previously used
        /// </summary>
        public bool WasUsed
        {
            get { return this.wasUsed; }
            set { this.wasUsed = value; }
        }

        /// <summary>
        /// Indicates how many times this instance is used
        /// </summary>
        public int Count
        {
            get { return this.count; }
            set { this.count = value; }
        }

        private long _offsetCntr = 0;
        public long offsetCntr
        {
            get { return _offsetCntr; }
            set { _offsetCntr = value; }
        }
        #endregion

        #region Constructor
        public DynamicVariable(String name)
        {
            this.wasUsed = name.StartsWith("+");
            this.name = Regex.Replace(name, @"[\ \+]", "");
            this.count = 0;
        }
        #endregion

        public override bool Equals(object obj)
        {
            if (obj is DynamicVariable)
                return ((DynamicVariable)obj).Name == this.name;
            else
                return false;
        }

        public override int GetHashCode()
        {
            return this.name.GetHashCode();
        }
    }

}
