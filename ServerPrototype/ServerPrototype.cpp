/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Arjun Kalyanasundaram, CSE687 - Object Oriented Design, Spring 2018 //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem-Windows/WindowsFileSys/WindowsFileSys.h"
#include <chrono>
#include"../CodePublisher/CodePublisher.h"

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

// --------------------<function to getfiles>----------------------------------------------
Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}

// --------------------<function to getDirs>----------------------------------------------
Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}

// --------------------<function  messsage echo>----------------------------------------------
std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};

// --------------------<function  to getfiles>----------------------------------------------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};
// --------------------<function  to get Directories>----------------------------------------------
std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};


// --------------------<function  getfile>----------------------------------------------
std::function<Msg(Msg)> getFile = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("file");
	reply.file(msg.attributes()["name"]);
  return reply;
};
// --------------------<function  to send arguments to Codepublisher>----------------------------------------------
std::vector<std::string> Connect(int argc, char **argv)
{
	CodePublisher pub;
	std::vector<std::string> value = pub.Conv_send(argc, argv);
	return value;
}

 Msg storen(std::vector<std::string> vec, Msg rn)
{ 
	size_t update = 0;
	for (auto holder : vec)
	{
		std::string value_num = Utilities::Converter<size_t>::toString(++update);
		rn.attribute("convert" + value_num, holder);
	}
	return rn;
}

 // --------------------<function definiton for message convert>----------------------------------------------
std::function<Msg(Msg)> convert = [](Msg msg) 
{
  Msg rep_con;
  std::string value_h= rep_con.attribValue(msg.attributes()["convert"]);  //creating message
  int argc = std::stoi(msg.attributes()["argc"]);
  std::vector<std::string> store ;
  for (int n = 1; n < argc + 1; n++)
  {
    std::string value_ch = std::to_string(n);
    std::string instr = "cmd" + value_ch;
    store.push_back(msg.attribValue(msg.attributes()[instr]));
  }
  std::vector<char*> reser_ch;
  reser_ch.reserve(store.size());
  for (size_t k = 0; k < store.size(); ++k)
  reser_ch.push_back(const_cast<char*>(store[k].c_str()));
  char** argv = &reser_ch[0];
  std::vector<std::string>  Publishn;
  Publishn = Connect(argc, argv);  //sending command line arguments to code publisher
  rep_con.to(msg.from());
  rep_con.from(msg.to());
  rep_con.command("convert");
  size_t update = 0;
  rep_con = storen(Publishn, rep_con);
  return rep_con;
};

// -----< demonstrating requirement 1 >----------------------------------------------
void demo1() {
	std::cout << ("----------<Demonstrating requirement 1>--------------\n");
	std::cout << ("Using  Visual Studio 2017 and its C++ Windows Console Projects \n")<<std::endl;
}
// -----< demonstrating requirement 2 >----------------------------------------------
void demo2() {
	std::cout << ("----------<Demonstrating requirement 2>--------------\n");
	std::cout << "Using Windows Presentation Foundation (WPF) for the Client's user display \n";
	std::cout << "Code Fragments: \n";
	std::cout << ("private string WDirectory { get; set; }\n") << std::endl;

	
}
// -----< demonstrating requirement 3 >----------------------------------------------
void demo3() 
{
	std::cout << ("----------<Demonstrating requirement 3>--------------\n");
	std::cout << "Assembled working parts from Projects #1, #2, and #3 into a Client-Server configuration.Here the Client does not wait for a reply from the Publisher server for a request before sending additional requests." << std::endl;
	std::cout << "\n";
}

// -----< demonstrating requirement 4 >----------------------------------------------
void demo4()
{
	std::cout << ("----------<Demonstrating requirement 4>--------------\n");
std::cout << "Provided a Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for conversion, and supports displaying the conversion similar to Project 3 \n";
std::cout << "GUI can be used to  input command  line arguments and converts files and  also sends files to the Client Folder\n" << std::endl;

}
// -----< demonstrating requirement 5>----------------------------------------------
void demo5()
{
	std::cout << ("----------<Demonstrating requirement 5>--------------\n");
	std::cout << "Provided message designs appropriate for this application. All messages are instances of the same Message class, but have a specified set of attributes and body contents suited for the intended task." << std::endl;
	std::cout << "Requirement demostrated by sending a  file to the client folder.\n" << std::endl;
}

// -----< demonstrating requirement 6 >----------------------------------------------
void demo6()
{
	std::cout << ("----------<Demonstrating requirement 6>--------------\n");
	std::cout << "Supports converting source code in the server and, with a separate request, transferring one or more converted files back to the local client, using the communication channel." << std::endl;
	std::cout << "\n";
}
// -----< demonstrating requirement 7>----------------------------------------------
void demo7() 
{
	std::cout << ("----------<Demonstrating requirement 7>--------------\n");
	std::cout << "Demonstrates correct operations for two or more concurrent clients." << std::endl;
	std::cout << "\n";
}

// -----< demonstrating requirement 8 >----------------------------------------------
void demo8() 
{
	std::cout << ("----------<Demonstrating requirement 8>--------------\n");
	std::cout << "Demonstration includes an automated test that accepts the server url and remote path to the project directory on its command line, invokes the Code Publisher, through its interface, to convert all the project files matching a pattern that accepts *.h and *.cpp files, and then it opens the Client GUI's Display view. \n" << std::endl;
}

// -----<Function to demonstrate all  requirements>----------------------------------------------
void demonstrations()
{
	demo1();
	demo2();
	demo3();
	demo4();
	demo5();
	demo6();
	demo7();
	demo8();
}
int main()
{
  demonstrations();
  std::cout << "\n  Testing Server Prototype";
  Server server(serverEndPoint, "ServerPrototype");
  server.start();
  std::cout << "\n  testing getFile  and Convert methods";
  std::cout << "\n --------------------------------------";
  Files files = server.getFiles();
  Dirs dirs = server.getDirs();
  std::cout << "\n";
  std::cout << "\n  testing the message processing";
  std::cout << "\n ----------------------------";
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("getFile", getFile);
  server.addMsgProc("convert", convert);
  server.addMsgProc("serverQuit", echo);
  server.processMessages();
  Msg msg(serverEndPoint, serverEndPoint);  // send to self
  msg.name("msgToSelf");
  msg.command("echo");
  msg.attribute("verbose", "show me");
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  msg.command("getFiles");
  msg.remove("verbose");
  msg.attributes()["path"] = storageRoot;
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  msg.command("getDirs");
  msg.attributes()["path"] = storageRoot;
  server.postMessage(msg);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "\n  press enter to exit";
  std::cin.get();
  std::cout << "\n";
  msg.command("serverQuit");
  server.postMessage(msg);
  server.stop();
  return 0;
}

