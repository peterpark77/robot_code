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

#ifndef VME_NMPC_VIRTUALMECOMMAND_HPP
#define VME_NMPC_VIRTUALMECOMMAND_HPP

#include "Nav2Robot.hpp"
#include <memory>

/**
 * Structures representing Nav2 commands. These objects will have a public
 * `execute(Nav2Robot&)` method so that they can be executed on a any given
 * connected Nav2Robot device. (See documentation for Nav2Robot for further
 * details.)
 */

struct VMeCommand {
  virtual int execute(Nav2Robot &) = 0;
};

struct VMeStop : public VMeCommand {
  virtual int execute(Nav2Robot &rob);
};

struct VMeV : public VMeCommand {
  float v = 0;
  float th = 0;
  float Dth = 0;

  VMeV(float th, float v, float Dth) : v{v}, th{th}, Dth{Dth} {}

  virtual int execute(Nav2Robot &rob);
};

struct VMeNullCmd : public VMeCommand {
  virtual int execute(Nav2Robot &rob);
};

using up_VMeCommand = std::unique_ptr<VMeCommand>;

#endif  // VME_NMPC_VIRTUALMECOMMAND_HPP
