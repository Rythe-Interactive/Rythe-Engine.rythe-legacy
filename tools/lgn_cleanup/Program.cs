using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace lgn_cleanup
{
    class Program
    {
        static int filesCleaned = 0;
        static int filesChecked = 0;

        static void Main(string[] args)
        {
            if (args.Length == 0 || args[0] == "-help" || args[0] == "-h")
            {
                Console.WriteLine(
                    "\t---lgn_cleanup---\n" +
                    "\ttool for automatically adding minor default items to legion source files.\n\n" +
                    "\tpropper usage: any argument is assumed to be a cleanup path except those starting with \"-ex=\",\n" +
                    "\t    arguments starting with \"-ex=\" are taken as exclusion patterns.\n" +
                    "\teg:\n" +
                    "\t    lgn_cleanup \"../legion\" -ex=\"**/glm/\" -ex=\"**/folder/*/file.hpp\""
                    );
                return;
            }

            Regex excludeRegex = new Regex("-ex=(.*)");
            Regex starReplace = new Regex("([^\\.*]*)\\*([^\\*]*)");

            List<string> searchPaths = new List<string>();
            List<Regex> excludePatterns = new List<Regex>();

            foreach (string command in args)
            {
                Match match = excludeRegex.Match(command);
                if (match.Success)
                {
                    string excludePattern = match.Groups[1].Value;
                    excludePattern = excludePattern.Replace("\"", "");
                    excludePattern = excludePattern.Replace("\\", "\\\\");
                    excludePattern = excludePattern.Replace("/", "\\\\");
                    excludePattern = excludePattern.Replace("**", ".*");
                    excludePattern = starReplace.Replace(excludePattern, "$1[^\\\\]*$2");

                    excludePatterns.Add(new Regex(excludePattern));
                }
                else
                {
                    if (!command.EndsWith('/') || !command.EndsWith('\\'))
                        searchPaths.Add(command + '/');
                    else
                        searchPaths.Add(command);
                }
            }

            foreach (string searchPath in searchPaths)
            {
                ProcessDir(searchPath, excludePatterns);
            }

            Console.WriteLine($"Checked {filesChecked} files and added {filesCleaned} headers in total.");
        }

        static void ProcessDir(string path, List<Regex> excludePatterns)
        {
            Console.WriteLine(path);

            string[] fileTypes = { "*.c", "*.h", "*.hpp", "*.cpp", "*.inl" };

            foreach (string type in fileTypes)
                foreach (String fileDir in Directory.GetFiles(path, type, SearchOption.AllDirectories))
                {
                    bool matched = false;
                    foreach (Regex regex in excludePatterns)
                        if (regex.IsMatch(fileDir))
                        {
                            matched = true;
                            break;
                        }

                    if (!matched)
                        CleanFile(path, fileDir);
                }
        }

        static string headerPattern = " \\* Copyright\\(c\\) [0-9]{4} Rythe-Interactive All rights reserved\\n";
        static Regex headerRegex = new Regex(headerPattern);

        static void CleanFile(string parent, string path)
        {
            try
            {
                bool writeContents = false;
                string contents;

                // Open the text file using a stream reader.
                using (var sr = new StreamReader(path))
                {
                    contents = sr.ReadToEnd();
                    filesChecked++;
                    Match match = headerRegex.Match(contents);
                    if (!match.Success)
                    {
                        writeContents = true;
                        bool isHeader = path.EndsWith(".hpp") || path.EndsWith(".h");

                        int day = DateTime.Now.Day;
                        int month = DateTime.Now.Month;
                        int year = DateTime.Now.Year;
                        string copyrightHeader =
                            "/*********************************************************************\n" +
                            " * @file   " + path.Replace(parent, "") + "\n" +
                            " * @date   " + day.ToString("D2") + "-" + month.ToString("D2") + "-" + year + "\n" +
                            " * @brief  This is an auto-generated file header.\n" +
                            " * \n" +
                            " * Copyright(c) " + year.ToString() + " Rythe-Interactive All rights reserved\n" +
                            " *********************************************************************/\n\n";

                        if (isHeader)
                        {
                            var sections = contents.Split(new String[] { "#pragma once\n" }, StringSplitOptions.RemoveEmptyEntries);
                            var pragmaCount = sections.Length - 1;

                            contents = "#pragma once\n" + copyrightHeader;
                            foreach (var section in sections)
                            {
                                if (section.StartsWith('\n'))
                                    contents += section.Substring(1);
                                else
                                    contents += section;
                            }
                        }
                        else
                            contents = copyrightHeader + contents;

                        filesCleaned++;
                        Console.WriteLine("\tAdded auto-generated file header to: " + path);
                    }
                }

                if (writeContents)
                    using (StreamWriter outputFile = new StreamWriter(path))
                    {
                        outputFile.Write(contents);
                    }
            }
            catch (IOException e)
            {
                Console.WriteLine("The file " + path + " could not be read:");
                Console.WriteLine(e.Message);
            }
        }
    }
}
