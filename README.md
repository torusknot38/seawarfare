# Sea Warfare Simulation

## Requirements

## Introduction

This project is a sea warfare simulator. The battle will take place on an open ocean with no land. The three objects available to the user are cruisers, aircraft carriers, and fighter planes. The user will command his or her ships and planes with orders contained in a file. A visualization of the battlefield and ship movement will be the output of the simulation.

## 1 Objects

The movable objects include cruisers, aircraft carriers, and fighters. All movable objects have the following properties:  
* Name  
* ID. ID must be unique  
* Max_speed. Speed is in MPH  
* Speed. Speed is in MPH  
* Heading. Heading is in degrees. See Location for more info.  
* Current Location. Location on the battlefield. See Location for more info.  
* All objects must keep track of where they have been since the beginning of the simulation.

### 1.1 Cruisers

A cruiser is the generic ship. In addition to the properties that apply to all movable objects, a cruiser has the following:
* Max_number_of_missiles. Maximum number of missiles a cruiser can carry.
* Cruisers can only move in two dimensions.

### 1.2 Aircraft Carriers

An aircraft carrier is a ship that can carry aircraft. In addition to the properties that apply to all movable objects, an aircraft carrier has the following:
* Max_number_of_aircraft. Maximum number of aircraft that an aircraft carrier can carry.
* Aircraft carriers do not need to be stationary for an aircraft to deploy or land.
* Aircraft carriers can only move in two dimensions.

### 1.3 Fighters

A fighter is a plane that can fly over the battlefield. In addition to the properties that apply to all movable objects, a fighter has the following:
* ShipID. The ID of the carrier that the fighter is to be initially placed on.
* Max_ceiling. Maximum altitude for the fighter in feet.
* Max_bombs. Maximum number of bombs that the fighter can carry.
* Fighters can move in three dimensions.
* Fighters must deploy from previously deployed aircraft carriers.
* Fighters do not need to land on the same carrier that it deployed from.

## 2 Orders

User input comes in the form of a text file composed of orders. An order consists of an opcode and necessary arguments. The order file will contain a list of orders – each order on a separate line. If the line begins with the # symbol or is blank, the line will be skipped. A clock will tick at least once per minute and the orders will be executed chronologically. For all orders the time will be formatted as follows: mm/dd/yyyy hh:mm:ss. There are four types of opcodes: sim management, object creation, object deployment, and object management.

### 2.1 Sim Management

 **2.1.1 StartSim**  
 Format: *StartSim day time*  
 Function: Begins the simulation according to the specified day and time.

**2.1.2 StopSim**  
Format: *StopSim day time*  
Function: Ends the simulation according to the specified day and time.

### 2.2 Object Creation

For an explanation of each argument, see each object’s respective section in section 1.

**2.2.1 CreateCruiser**  
Format: *CreateCruiser Name ID Max_number_of_missiles Max_speed*  
Function: Creates a cruiser but does not deploy it anywhere on the battlefield.

**2.2.2 CreateAirCraftCarrier**  
  Format: *CreateAirCraftCarrier Name ID Max_number_of_aircraft Max_speed*  
  Function: Creates an aircraft carrier but does not deploy it anywhere on the battlefield.

**2.2.3 CreateFighter**  
	Format: *CreateFighter Name ID ShipID Max_speed Max_ceiling Max_bombs*  
	Function: Creates a fighter and places it on the deployed ship specified.

### 2.3 Object Deployment

**2.3.1 DeployShip**  
Format: *DeployShip day time ID start_x start_y heading speed*  
Function: Places a created ship on a specified point (start_x, start_y) on the battlefield. The ship is also given a heading and speed. See Location and Movement for more info.

**2.3.2 DeployAircraft**  
Format: *DeployAircraft day time ID heading speed altitude*  
Function: Launches the aircraft from its carrier. The aircraft is also given a heading, speed, and altitude. See Location and Movement for more info.

### 2.4 Object Management

**2.4.1 ChangeShipOrders**  
Format: *ChangeShipOrders day time ID heading speed*  
Function: Changes the specified ship’s heading and speed. If the heading or speed are set to -1, then the current heading or speed will not be changed.

**2.4.2 ChangeAircraftOrders**  
Format: *ChangeAircraftOrders day time ID heading speed altitude*  
Function: Changes the specified aircraft’s heading, speed, and altitude. If the heading, speed, or altitude are set to -1, then the current heading, speed, or altitude will not be changed.

**2.4.3 LandAircraft**  
Format: *LandAircraft day time shipID aircraftID*  
Function: Targets the desired carrier (shipID) to land on. Does not need to land on the same ship from which it deployed. Once within a specific radius, the aircraft will land and therefore is no longer deployed. If the carrier has reached its Max_number_of_aircraft, the aircraft will not be allowed to land, and must find a different carrier with enough space.

## 3 Location and Movement

* The simulation will take place on a Cartesian coordinate plane with each coordinate in the form (x,y).
* The point (0,0) will be the center of the screen.
* Altitude is the number of feet above the sea, where sea level is at altitude = 0.
* Headings are compass headings in degrees where 0 degrees is due North, 90 degrees is due East, 180 degrees is due South, and 270 degrees is due West.
* After each tick on the simulator’s clock, each object must update its Current Location.
* Current Location is determined by the process of “dead reckoning.” The object’s location can be determined using its heading, speed, the elapsed time since last location, and some trigonometry.
* Objects may change heading and speed instantly.

## 4 Simulation Output

The output of the simulation will be either a printout of all of the objects’ locations or a simple graphics simulation.
