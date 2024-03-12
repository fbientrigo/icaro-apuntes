Crear una clase tipo:
```python
# Main class to represent the player's physics body. Controls the player's
# current health and how to operate when an upgrade choice has been made.
class_name PlayerShip
extends CharacterBody2D

signal died

@export var stats: Resource = preload("res://Ships/Player/player_stats.tres")
@export (int, LAYERS_2D_PHYSICS) var projectile_mask := 0
@export var ExplosionEffect: PackedScene
# Represents the ship on the minimap. Use a MapIcon resource.
@export var map_icon: Resource

var dockables := []

@onready var shape := $CollisionShape3D
@onready var agent: GSAISteeringAgent = $StateMachine/Move.agent
@onready var camera_transform := $CameraTransform
@onready var timer := $MapTimer
@onready var cargo := $Cargo
@onready var move_state := $StateMachine/Move
@onready var gun := $Gun
@onready var laser_gun := $LaserGun
@onready var vfx := $VFX


func _ready() -> void:
	stats.initialize()
	Events.connect("damaged",Callable(self,"_on_damaged"))
	Events.connect("upgrade_chosen",Callable(self,"_on_upgrade_chosen"))
	stats.connect("health_depleted",Callable(self,"die"))
	gun.collision_mask = projectile_mask
	laser_gun.collision_mask = projectile_mask


func _toggle_map(map_up: bool, tween_time: float) -> void:
	if not map_up:
		timer.start(tween_time)
		await timer.timeout
	camera_transform.update_position = not map_up


func die() -> void:
	var effect := ExplosionEffect.instantiate()
	effect.global_position = global_position
	ObjectRegistry.register_effect(effect)

	emit_signal("died")
	Events.emit_signal("player_died")

	queue_free()


func grab_camera(camera: Camera2D) -> void:
	camera_transform.remote_path = camera.get_path()


func _on_damaged(target: Node, amount: int, _origin: Node) -> void:
	if not target == self:
		return

	stats.health -= amount


func _on_upgrade_chosen(choice: int) -> void:
	match choice:
		Events.UpgradeChoices.HEALTH:
			stats.add_modifier("max_health", 25.0)
		Events.UpgradeChoices.SPEED:
			stats.add_modifier("linear_speed_max", 125.0)
		Events.UpgradeChoices.CARGO:
			cargo.stats.add_modifier("max_cargo", 50.0)
		Events.UpgradeChoices.MINING:
			cargo.stats.add_modifier("mining_rate", 10.0)
			cargo.stats.add_modifier("unload_rate", 5.0)
		Events.UpgradeChoices.WEAPON:
			gun.stats.add_modifier("damage", 3.0)
			# modifiers only add or remove values, is incapable of lims
			if gun.stats.get_stat("cooldown") > 0.2:
				gun.stats.add_modifier("cooldown", -0.05)

```


Unas formas de combinar la clase `State` y un State Machine
```python
# State interface to use in Hierarchical State Machines.
# The lowest leaf tries to handle callbacks, and if it can't, it delegates the work to its parent.
# It's up to the user to call the parent state's functions, e.g. `_parent.physics_process(delta)`
# Use State as a child of a StateMachine node.
# tags: abstract
extends Node
class_name State

@onready var _state_machine := _get_state_machine(self)
var _parent: State = null


func _ready() -> void:
	await owner.ready
	_parent = get_parent() as State
	

func unhandled_input(_event: InputEvent) -> void:
	pass


func physics_process(_delta: float) -> void:
	pass


func enter(_msg: Dictionary = {}) -> void:
	pass


func exit() -> void:
	pass


func _get_state_machine(node: Node) -> Node:
	if node != null and not node.is_in_group("state_machine"):
		return _get_state_machine(node.get_parent())
	return node

```

```python
# Generic State Machine. Initializes states and delegates engine callbacks
# (_physics_process, _unhandled_input) to the active state.
extends Node
class_name StateMachine

@export var initial_state := NodePath()

@onready var state: State = get_node(initial_state) : set = set_state
@onready var _state_name := state.name


func _init():
	add_to_group("state_machine")


func _ready() -> void:
	await owner.ready
	state.enter()


func _unhandled_input(event: InputEvent) -> void:
	state.unhandled_input(event)


func _physics_process(delta: float) -> void:
	state.physics_process(delta)


func transition_to(target_state_path: String, msg: Dictionary = {}) -> void:
	if not has_node(target_state_path):
		return

	var target_state := get_node(target_state_path)

	state.exit()
	self.state = target_state
	state.enter(msg)


func set_state(value: State) -> void:
	state = value
	_state_name = state.name

```
