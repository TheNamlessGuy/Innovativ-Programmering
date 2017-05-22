#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <algorithm>
#include <errno.h>

using namespace std;

void string_to_lowercase(string& str) {
  transform(str.begin(), str.end(), str.begin(), ::tolower);
}

vector<string> read_file(string path) {
  ifstream ifs(path);
  vector<string> return_vector;
  for (string line; getline(ifs, line);) {
    return_vector.push_back(line);
  }
  return return_vector;
}

bool no_bad_words(string str, string path) {
  vector<string> bad_words = read_file(path);
  string_to_lowercase(str);

  for (string bad_word : bad_words) {
    if (str.find(bad_word) != string::npos) {
      return false;
    }
  }
  return true;
}

string recieve_packet(int new_sd) {
  string packet = "";
  char incoming_data_buffer[1000];
  int bytes_recieved;
  
  while (true) { //RECIEVE LOOP
    bytes_recieved = recv(new_sd, incoming_data_buffer,
                          sizeof(incoming_data_buffer), 0);

    packet += string(reinterpret_cast<const char*>(incoming_data_buffer),
                     bytes_recieved);
    
    cout << "SERVER: " << bytes_recieved << " bytes recieved" << endl;

    if (bytes_recieved == 0) {
      cout << "SERVER: Host disconnected" << endl;
      break;
    } else if (bytes_recieved < sizeof(incoming_data_buffer)) {
      break;
    } else if (bytes_recieved == -1) {
      cerr << "SERVER: Recv error" << endl;
      exit(1);
    }
  }
  return packet;
}

int get_addr_info(string port, struct addrinfo host_hints,
                  struct addrinfo*& host_info_list) {
  int status = getaddrinfo(NULL, port.c_str(), &host_hints, &host_info_list);
  if (status != 0) {
    cerr << "getaddrinfo error" << gai_strerror(status) << endl;
    exit(1);
  }
  return status;
}

int start_socket(struct addrinfo*& host_info_list) {
  int socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                        host_info_list->ai_protocol);
  if (socketfd == -1) {
    cerr << "socket error" << endl;
    exit(1);
  }

  return socketfd;
}

int setsocketoptions(int socketfd) {
  int yes = 1;
  int status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if (status != 0) {
    cerr << "setsockopt error (SO_REUSEADDR)" << endl;
    exit(1);
  }
  return status;
}

int bind_port(int socketfd, struct addrinfo* host_info_list) {
  int status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "bind error" << endl;
    exit(1);
  }
  return status;
}

string get_host(string packet) {
  string host = packet;
  string_to_lowercase(host);
  host = host.substr(host.find("host: ") + 6);
  host = host.substr(0, host.find("\r\n"));
  return host;
}

string get_requested_path(string packet) {
  string req_path = packet;
  string_to_lowercase(req_path);
  req_path = req_path.substr(req_path.find("get ") + 4);
  req_path = req_path.substr(0, req_path.find(" "));
  return req_path;
}

string get_url(string packet) {
  return get_host(packet) + get_requested_path(packet);
}

void set_connection_to_close(string& packet) {
  string temp_str = packet;
  string_to_lowercase(temp_str);
  if (temp_str.find("proxy-connection: ") != string::npos) {
    temp_str.replace(temp_str.find("proxy-connection: "), 18, "connection: ");
  }
  int connection_point = temp_str.find("connection: ") + 12;
  temp_str = temp_str.substr(connection_point);
  temp_str = temp_str.substr(0, temp_str.find("\r\n"));
  packet.replace(connection_point, temp_str.length(), "close");
}

void fix_header(string& packet) {
  string temp_str = get_requested_path(packet);
  if (temp_str.find("http://") != string::npos) {
    temp_str = temp_str.substr(temp_str.find("http://") + 7);
    packet.replace(packet.find("http://"), temp_str.find("/") + 7, "");
  }
  set_connection_to_close(packet);
}

string client_actions(string getpacket) {
  //cout << "CLIENT: Packet to send:" << endl
  //     << getpacket << endl;
  string recievedpacket = "";

  int status = 0;
  int socketfd = 0;

  struct addrinfo host_hints;
  struct addrinfo* host_info_list;

  ssize_t bytes_sent = 0;
  ssize_t bytes_recieved = 0;
  char incoming_data_buffer[1000];

  memset(&host_hints, 0, sizeof(host_hints));

  host_hints.ai_family = AF_UNSPEC; //IPv4 || IPv6
  host_hints.ai_socktype = SOCK_STREAM; //TCP
  
  string url = get_host(getpacket);
  cout << "CLIENT: URL -> " << '\'' + url + '\'' << endl;
  
  status = getaddrinfo(url.c_str(), "80", &host_hints, &host_info_list);
  if (status != 0) {
    cerr << "CLIENT: Getaddrinfo error: " << gai_strerror(status) << endl;
    return "";
  }
  
  socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                    host_info_list->ai_protocol);
  if (socketfd == -1) {
    cerr << "CLIENT: Socket error" << endl;
    exit(1);
  }
  
  status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1) {
    cerr << "CLIENT: Connect error" << endl;
    exit(1);
  }

  int yes = 1;
  status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if (status != 0) {
    cerr << "CLIENT: Setsockopt error" << endl;
    exit(1);
  }

  cout << "CLIENT: Sending message" << endl;
  
  int total_bytes_sent = 0;
  while (true) {
    bytes_sent = send(socketfd, getpacket.c_str(), getpacket.length(), MSG_NOSIGNAL);
    total_bytes_sent += bytes_sent;
    cout << "CLIENT: Sent " << bytes_sent << " bytes." << endl;

    
    if (bytes_sent == -1) {
      cerr << "CLIENT: Send error" << endl;
      cerr << "errno: " << errno << " (32 means pipe is closed)" << endl;
      exit(1);
    }
    if (total_bytes_sent >= getpacket.length()) {
      cout << "CLIENT: Sending done" << endl;
      break;
    }
  }

  cout << "CLIENT: Waiting to recieve data..." << endl;
  while (true) {
    bytes_recieved = recv(socketfd, incoming_data_buffer,
                          sizeof(incoming_data_buffer), 0);

    string incoming_data_string(reinterpret_cast<const char*>(incoming_data_buffer),
                                bytes_recieved);
    recievedpacket += incoming_data_string;
    
    if (bytes_recieved == 0) {
      cout << "CLIENT: Close by disconnect" << endl;
      break;
    } else if (bytes_recieved < incoming_data_string.size()) {
      cout << "CLIENT: Break by size" << endl;
      break;
    } else if (bytes_recieved == -1) {
      cout << "CLIENT: Recv error" << endl;
      exit(1);
    }
  }

  freeaddrinfo(host_info_list);
  close(socketfd);
  //cout << "CLIENT: Packet recieved:" << endl
  //     << recievedpacket << endl;
  return recievedpacket;
}

string get_redirect_packet(string host, string path) {
  return "HTTP/1.1 302 Found\r\nLocation: http://" + host + path + "\r\n\r\n";
}

string get_content_type(string packet) {
  string pkt = packet;
  string_to_lowercase(pkt);
  
  pkt = pkt.substr(pkt.find("content-type: ") + 14);
  pkt = pkt.substr(0, pkt.find("\r\n"));
  return pkt;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "USAGE: ./proxyserver PORT" << endl;
    return 1;
  }
  
  char* port = argv[1];
  string packet = "";

  int status = 0;
  int socketfd = 0;
  int new_sd;

  struct addrinfo host_hints;
  struct addrinfo* host_info_list;

  struct sockaddr_storage their_addr;
  socklen_t their_addr_size = sizeof(their_addr);

  ssize_t bytes_sent;

  cout << "Starting proxyserver..." << endl;
  
  memset(&host_hints, 0, sizeof(host_hints));

  host_hints.ai_family = AF_UNSPEC; //IPv4 || IPv6
  host_hints.ai_socktype = SOCK_STREAM; //TCP
  host_hints.ai_flags = AI_PASSIVE; //Socket will be bound

  status = get_addr_info(port, host_hints, host_info_list);

  socketfd = start_socket(host_info_list);
  status = setsocketoptions(socketfd);
  
  status = bind_port(socketfd, host_info_list);
  while (true) { //MAIN LOOP
    status = listen(socketfd, 1); //Max connections is 2 (current connection + 1 backlog)
    if (status == -1) {
      cerr << "SERVER: Listen error" << endl;
      return 1;
    }
    
    new_sd = accept(socketfd, (struct sockaddr*)&their_addr, &their_addr_size);
    if (new_sd == -1) {
      cerr << "SERVER: Listen error" << endl;
      return 1;
    }
  
    cout << "SERVER: Connection established" << endl
         << "SERVER: Waiting for data..." << endl;
    packet = recieve_packet(new_sd);
    if (packet.length() > 0) {
      fix_header(packet);
      
      string recievedpacket = "";
      if (!no_bad_words(get_url(packet), "./badwordsURL.txt")) {
        recievedpacket = get_redirect_packet("www.ida.liu.se", "/~TDTS04/labs/2011/ass2/error1.html");
      } else {
        recievedpacket = client_actions(packet);
        
        if (recievedpacket.length() > 0 &&
            get_content_type(recievedpacket).find("text") != string::npos &&
            !no_bad_words(recievedpacket, "./badwordsBody.txt")) {
          recievedpacket = get_redirect_packet("www.ida.liu.se", "/~TDTS04/labs/2011/ass2/error2.html");
        }
      }
      
      if (recievedpacket.length() <= 0) {
        cout << "SERVER: 404 Not Found" << endl;
        recievedpacket = "HTTP/1.1 404 Not Found\r\n\r\n<b><font size='20'><font color='red'>404</font> Not Found</font></b></br>The URL can not be reached";
      }
      int totalbyteslength = 0;
      while (true) {
        bytes_sent = send(new_sd, recievedpacket.c_str(), recievedpacket.length(), 0);
        totalbyteslength += bytes_sent;

        if (totalbyteslength >= recievedpacket.length()) {
          cout << "SERVER: Packet sent" << endl;
          break;
        }
      }
    }

    cout << "SERVER: Connection done" << endl << endl;
    close(new_sd);
  }
  cout << "SERVER: Shutting down..." << endl;
  freeaddrinfo(host_info_list);
  close(socketfd);

  return 0;
}
