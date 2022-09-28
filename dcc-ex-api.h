/*
 *  © 2022 Peter Cole
 *
 *  This file is part of the DCC-EX API library for Arduino.
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Turntable-EX.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DCC_EX_API_H
#define DCC_EX_API_H

void setTrackPower(bool powerState);
void setJoin();
void setLocoThrottle(uint16_t locoAddress, int8_t speed, bool direction);
void setForgetAllLocos();
void setForgetLoco(uint16_t locoAddress);
void setEstopAll();
void setLocoFunction(uint16_t locoAddress, uint8_t function, bool state);
void setAccActivateLinear(uint16_t linearAddress, bool state);
void setAccActivateAddr(uint16_t address, uint8_t subaddress, bool state);
void getAllTurnouts();
void getTurnout(uint16_t turnoutID);
void setTurnout(uint16_t turnoutID, bool position);
void getAllRoutes();
void getRoute(uint16_t routeID);
void setStartRoute(uint16_t locoAddress, uint16_t routeID);
void getRosterEntries();
void getRoster(uint16_t rosterID);
void parseAPICommand(char* serialInput, uint8_t arraySize);

#endif
