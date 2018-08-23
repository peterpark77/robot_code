/* This file is part of vme-nmpc.
 *
 * Copyright (C) 2015 Timothy A.V. Teatro - All rights Reserved
 *
 * vme-nmpc is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * vme-nmpc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * vme-nmpc. If not, see <http://www.gnu.org/licenses/>.
 */

/* This file is based on the work of J-P Gignac. It is basically a rewrite of
 * his daemon class from the telep-head source tree, found in
 *  telep-head/unwarp/Daemon.cpp
 */

#ifndef VME_NMPC_SRC_DAEMON_HPP_
#define VME_NMPC_SRC_DAEMON_HPP_

/**
 * For historical AND practical reasons, I use the name daemon instead of
 * server. The word server in this class refers to the functions which handle
 * the _serving_ of requests while the daemon is the process that runs in the
 * background, creates the socket and owns the threads that do the serving.
 *
 * An object of type Daemon will spawn a thread that runs daemon_threadfn(). The
 * constructor of Daemon requires a port number and a serverChild function. The
 * Daemon object will create and bind a socket and listen on the specified port.
 *
 * The thread function daemon_threadfn() maintains a list, ticketStubs, of
 * objects of type RequestTicket. The function runs in a loop and in each
 * iteration, it emplaces_back() a new RequestTicket on the list. The
 * constructor of a RequestTicket calls accept(), which blocks execution in the
 * thread until a connection is made. Once the connection is made, the
 * construction continues wherein a new thread is spawned which runs
 * server_threadfn() which executes the serverChild() function. When the server
 * child terminates, the RequestTicket is marked done and is cleaned up in the
 * next iteration of daemon_threadfn().
 *
 *
 * Here is a diagram illustrating the relationships described in the previous
 * paragraph:
 *
 * ----------------------------------------------------------------------------
 *    @ Thread lifetime,      +-> Object Ownhership/Function call
 *    |
 *    * End of thread
 * ----------------------------------------------------------------------------
 *
 * Daemon(.)
 *  @ Daemon server thread
 *  | +->std::list<RequestTicket> ticketStubs
 *  |    +-> ticket_stubes.emplace_back(.) -> RequestTicket(.)
 *  |    +   +-> @ server_child_wrapper
 *  |    +       | +->Daemon.serverChild(.)
 *  |    +-> ticket_stubes.emplace_back(.) -> RequestTicket(.)
 *  |    +   +-> @ server_child_wrapper
 *  |    +       | +->Daemon.serverChild(.)
 *  |   ...
 *  |    +-> ticket_stubes.emplace_back(.) -> RequestTicket(.)
 *  |        +-> @ server_child_wrapper
 *  |            | +->Daemon.serverChild(.)
 *  *
 */

#include <thread>

class Daemon {
  std::function<void(int)> serverChild;
  bool shutdownFlag;
  int sockfd;
  std::thread daemonThread;

  friend void daemon_threadfn(const Daemon*);
  friend class RequestTicket;

 public:
  Daemon(const Daemon&) = delete;
  Daemon& operator=(const Daemon&) = delete;
  Daemon(int port, std::function<void(int)>);
  ~Daemon();
};

class RequestTicket {
  const Daemon* parentDaemon;
  int connectionfd;
  std::thread serverThread;
  friend void server_child_wrapper(RequestTicket*);
  auto get_serverChild() { return parentDaemon->serverChild; }

 public:
  bool done;
  RequestTicket(const Daemon*);
  ~RequestTicket();
};

/**
 * An exception class to throw from RequestTicket's constructor. See
 * documentation therein for motivation.
 */
struct BlockedSocket : public std::exception {
  char const* what() const throw() { return ""; };
};

#endif  // VME_NMPC_SRC_DAEMON_HPP_
