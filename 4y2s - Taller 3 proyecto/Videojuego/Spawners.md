```python
# Spawns a group of pirates, then updates their squad and faction. Pirates are
# spawned outside of the asteroid belt, but are asigned an asteroid cluster
# or a point in space in the belt for them to path to. This keeps pirates from
# appearing right on top of the player, or to appear jarringly out of thin air.
class_name PirateSpawner
extends Node2D

@export var PirateScene: PackedScene
@export var count_min := 1
@export var count_max := 5
@export var spawn_radius := 150.0


func spawn_pirate_group(
	rng: RandomNumberGenerator, _choice: int, world_radius: float, cluster_position: Vector2
) -> void:
	var spawn_position := cluster_position.normalized() * world_radius * 1.25

	var pirates_in_cluster := []
	for _i in range(rng.randi_range(count_min, count_max)):
		var pirate := PirateScene.instantiate()
		pirate.position = (
			spawn_position
			+ Vector2.UP.rotated(rng.randf_range(0, PI * 2)) * spawn_radius
		)
		pirates_in_cluster.append(pirate)
	for p in pirates_in_cluster:
		p.setup_squad(
			p == pirates_in_cluster[0], pirates_in_cluster[0], cluster_position, pirates_in_cluster
		)
		p.setup_faction(get_tree().get_nodes_in_group("Pirates"))
		add_child(p)
		Events.emit_signal("pirate_spawned", p)

```

Spawner de asteroides
```python
# Dots a cluster of asteroids in the world, reporting the amount of iron added
# to the world via signals. The asteroids are spawned in groups at a random
# point inside of an asteroid belt that lies a minimum distance from the player
# station's spawn point. Most of the logic is just making sure that a cluster
# doesn't appear too close to another.
class_name AsteroidSpawner
extends Node2D

signal cluster_depleted(iron_left)

@export var count_min := 1
@export var count_max := 5

@export var min_distance_from_station := 800.0
@export var min_distance_between_clusters := 600.0

@export var cluster_radius := 150.0
@export var asteroid_radius := 75.0
@export var randomize_rotation := true


# Spawns new asteroids until there's enough resources to mine in the world.
# The target amount of resources is `iron_amount_balance_level`.
func spawn_asteroid_clusters(
	rng: RandomNumberGenerator, target_iron_amount: float, world_radius: float
) -> float:
	var spawned_iron := 0.0
	while spawned_iron < target_iron_amount:
		var cluster := _spawn_asteroid_cluster(rng, world_radius)
		spawned_iron += cluster.iron_amount
	return spawned_iron


# Returns the remaining amount of resources to mine on the map.
func calculate_remaining_iron() -> float:
	var sum := 0.0
	for cluster in get_children():
		sum += cluster.iron_amount
	return sum


# Generates and randomly places a new asteroid cluster, then returns the newly created instance.
func _spawn_asteroid_cluster(rng: RandomNumberGenerator, world_radius: float) -> AsteroidCluster:
	var new_cluster: AsteroidCluster
	while not new_cluster:
		var spawn_position := (
			Vector2.UP.rotated(rng.randf_range(0, PI * 2))
			* rng.randf_range(min_distance_from_station, world_radius)
		)
		for cluster in get_children():
			if (
				spawn_position.distance_squared_to(cluster.position)
				< pow(min_distance_between_clusters, 2)
			):
				continue
		new_cluster = _create_cluster(rng, spawn_position)
		break
	return new_cluster


# Creates, initializes, and returns a new cluster with its asteroids pre-generated
func _create_cluster(rng: RandomNumberGenerator, spawn_position: Vector2) -> AsteroidCluster:
	var cluster := AsteroidCluster.new()
	add_child(cluster)
	cluster.global_position = spawn_position
	cluster.spawn_asteroids(rng, count_min, count_max, cluster_radius, asteroid_radius)
	return cluster


func _on_AsteroidCluster_depleted() -> void:
	emit_signal("cluster_depleted", calculate_remaining_iron())

```



Station spawner, para hacer spawn del player
```python
# Spawns the player and the resource offloading station at the center of the
# world.
class_name StationSpawner
extends Node2D

@export var Station: PackedScene
@export var radius_player_near_station := 300.0

@onready var player_ship: PlayerShip = $PlayerShip


func spawn_station(rng: RandomNumberGenerator) -> void:
	var station := Station.instantiate()
	add_child(station)
	player_ship.global_position = (
		station.global_position
		+ (Vector2.UP.rotated(rng.randf_range(0, PI * 2)) * radius_player_near_station)
	)
	Events.emit_signal("station_spawned", station, player_ship)


func get_player() -> PlayerShip:
	return player_ship

```

