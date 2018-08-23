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

#include "CliHandler.hpp"
#include "ObstacleTypes/PointObstacle.hpp"

#include <unistd.h>

std::string detach_token(std::string&);
std::string fetch_message_string(const int);
void lowercase(std::string&);

CliHandler::CliHandler(TargetContainer* tgt, ObstacleContainer* obs)
    : targets(tgt), obstacles(obs) {}

CliHandler::CliHandler(CliHandler&& source) {
  targets = source.targets;
  source.targets = nullptr;
  obstacles = source.obstacles;
  source.obstacles = nullptr;
}

CliHandler::CliHandler(const CliHandler& source) {
  targets = source.targets;
  obstacles = source.obstacles;
}

void CliHandler::operator()(const int sockfd) {
  for (;;) {
    std::string line = fetch_message_string(sockfd);
    auto cmd = detach_token(line);
    lowercase(cmd);
    // TODO: Add e-stop.
    if (cmd == "at")
      add_target(line);
    else if (cmd == "ao")
      add_obstacle(line);
    else if (cmd == "clear")
      clear(line);
    else if (cmd == "eot")
      return;
  }
}

void CliHandler::add_target(std::string line) {
  fptype x{0}, y{0}, tol{0};
  try {
    x = std::stof(detach_token(line));
    y = std::stof(detach_token(line));
    tol = std::stof(detach_token(line));
  } catch (std::invalid_argument) {
    return;
  }
  targets->emplace_back(new Target(x, y, tol));
}

void CliHandler::add_obstacle(std::string line) {
  auto obstacleType = detach_token(line);
  fptype x{0}, y{0}, pwr{0}, eps{0};
  if (obstacleType == "PointObstacle") {
    try {
      x = std::stof(detach_token(line));
      y = std::stof(detach_token(line));
      pwr = std::stof(detach_token(line));
      eps = std::stof(detach_token(line));
    } catch (std::invalid_argument) {
      return;
    }
    obstacles->push(new PointObstacle{fp_point2d{x, y}, pwr, eps});
  }
}

void CliHandler::clear(std::string line) {
  auto whatToClear = detach_token(line);
  if (whatToClear == "targets") targets->clear();
  if (whatToClear == "obstacles") obstacles->clear();
  if (whatToClear == "all") {
    targets->clear();
    obstacles->clear();
  }
}

std::string detach_token(std::string& line) {
  unsigned begin = 0;
  while (begin < line.length() && std::isspace(line[begin])) ++begin;
  unsigned end = begin;
  while (end < line.length() && !std::isspace(line[end])) ++end;
  std::string token = line.substr(begin, end - begin);
  line = line.substr(end, line.length() - end);
  return token;
}

std::string fetch_message_string(const int sockfd) {
  char buff[120];
  if (read(sockfd, &buff, 80) < 1) return "eot";
  return std::string{buff};
}

void lowercase(std::string& string) {
  for (auto& each : string) each = std::tolower(each);
}
