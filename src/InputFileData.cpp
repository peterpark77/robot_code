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

#include "InputFileData.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>

namespace pt = boost::property_tree;

class ErrorReadingNmpcHorizonSizeFromInputFile : public std::exception {
  virtual const char* what() const noexcept override {
    return "Error reading nmpcHorizonSize from Input File";
  }
};

void InputFileData::load(const std::string& filename) {
  pt::ptree tree;
  pt::read_json(filename, tree);

  try {
    nmpcHorizon = tree.get<unsigned>("nmpcHorizon");
  } catch (...) {
    throw ErrorReadingNmpcHorizonSizeFromInputFile();
  }
  timeInterval = tree.get<float>("timeInterval");
  cruiseSpeed = tree.get<float>("cruiseSpeed");
  Q = tree.get<float>("Q");
  Q0 = tree.get<float>("Q0");
  R = tree.get<float>("R");
  sdConvergenceTolerance = tree.get<float>("sdConvergenceTolerance");
  sdStepFactor = tree.get<float>("sdStepFactor");
  maxSdSteps = tree.get<unsigned>("maxSdSteps");
  targetDistanceTolerance = tree.get<float>("targetDistanceTolerance");
  jsonLogPath = tree.get<std::string>("jsonLogPath");
}
