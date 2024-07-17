# API-project-2023
Final project of the course "Algoritmi e Principi dell'Informatica", Politecnico di Milano, AY 2022-2023

[📖 Requirements](/requirements.pdf)

✅ Score: 30 cum laude


## Project Description

### Overview

This project focuses on a highway described as a sequence of service stations, each uniquely identified by its distance from the highway's starting point. Each service station offers a fleet of electric vehicles with specified battery ranges. The main objective is to plan the route with the fewest stops between two given stations, adhering to the constraints provided.

### Key Features

- **Add and Remove Service Stations**: Manage the addition and removal of service stations along the highway.
- **Manage Vehicle Fleets**: Add or remove electric vehicles with specific ranges at each station.
- **Optimal Route Planning**: Plan the shortest route between two stations, considering the constraints and vehicle ranges.

### Constraints

1. Each service station is uniquely identified by its distance from the start of the highway.
2. The fleet of vehicles at any station can have up to 512 vehicles.
3. A journey starts at one station and ends at another, with no backtracking allowed.
4. The vehicle for each leg of the journey is rented at the current station.
5. If multiple routes have the same number of stops, the route favoring stations closer to the start is chosen.

### Example

Given an example highway with stations at distances 20, 30, 45, and 50 kilometers, and a set of vehicles with various ranges, the correct route from the station at 20 km to the station at 50 km would be:

`20 → 30 → 50`

And not:

`20 → 45 → 50`

## Commands

The input file contains a sequence of commands, one per line, formatted as follows. All integer values are 32-bit positive or zero.

### Add a Station

`aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n`

Example

`aggiungi-stazione 10 3 100 200 `


Adds a station at 10 km from the start, with a fleet of three vehicles having ranges 100 km, 200 km, and 300 km, respectively. If a station already exists at the specified distance, the command does nothing.

### Remove a Station

`demolisci-stazione distanza`


Example:

`demolisci-stazione 10`


Removes the station at 10 km if it exists.

### Add a Vehicle

`aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere`


Example:

`aggiungi-auto 10 400`

Adds a vehicle with a 400 km range to the station at 10 km if the station exists.

### Remove a Vehicle

`rottama-auto distanza-stazione autonomia-auto-da-rottamare`

Example:

`rottama-auto 10 200`

Removes a vehicle with a 200 km range from the station at 10 km if the station exists and has such a vehicle.

### Plan a Route

`pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo`

Example:

`pianifica-percorso 20 50`

Plans the route from the station at 20 km to the station at 50 km, adhering to the constraints. Outputs the sequence of stops, or "nessun percorso" if no valid route exists.
