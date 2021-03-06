﻿#pragma once


//Các thông số của MARIO
//Các transformation
#define	MARIO_SMALL_FORM				0
#define	MARIO_BIG_FORM					1
#define MARIO_FIRE_FORM					2
#define MARIO_RACCOON_FORM				3

// Chiều cao và rộng ở các form
#define MARIO_BIG_BBOX_WIDTH			16
#define MARIO_BIG_BBOX_HEIGHT			27
#define MARIO_SMALL_BBOX_WIDTH			16
#define MARIO_SMALL_BBOX_HEIGHT			16
#define MARIO_FIRE_BBOX_WIDTH			16
#define MARIO_FIRE_BBOX_HEIGHT			27
#define MARIO_RACCOON_BBOX_WIDTH		16
#define MARIO_RACCOON_BBOX_HEIGHT		27
#define MARIO_BBOX_SQUAT_HEIGHT			18
#define MARIO_BBOX_SQUAT_WIDTH			14
#define MARIO_TAIL_BBOX_WIDTH			9


//Các chỉ số thời gian và tốc độ

#define MARIO_UNTOUCHABLE_TIME			1000
#define MARIO_WALKING_SPEED				0.1f
#define MARIO_JUMP_SPEED_Y				0.2f
#define MARIO_SUPER_JUMP_SPEED			0.4f
#define MARIO_FLYING_SPEED				0.3f
#define MARIO_MAX_JUMPING_STACK			19
#define MARIO_JUMP_DEFLECT_SPEED		0.3f
#define MARIO_WALK_DEFELCT_SPEED		0.1f
#define MARIO_BRAKE_DEFLECT_SPEED		0.08f
#define MARIO_GRAVITY					0.002f
#define MARIO_LOWER_GRAVITY				0.001f
#define MARIO_DIE_DEFLECT_SPEED			0.5f
#define BUFF_SPEED						0.008f	// tốc độ tăng lên khi tích stack
#define POWER_MELTER_FULL				7		// số stack tối đa		//thời gian để tích 1 stack
#define POWER_MELTER_MINIMUM			0
#define POWER_MELTER_BUFF_SPEED_LEVEL	3
#define STACK_TIME						200			//thời gian để tích 1 stack
#define LOSE_STACK_TIME					300
#define MARIO_FRICTION					0.05f
#define MARIO_SUPER_JUMP_TIME			150
#define MARIO_FLYING_LIMITED_TIME		4000
#define MARIO_FLOATING_SPEED_Y			0.0f
#define MARIO_MAX_BULLET				2
#define MARIO_SHOOTING_TIME				200
#define MARIO_TAIL_ATTACK_TIME			300
#define MARIO_EACH_STAGE_IN_SWING_TAIL_TIME	50
#define MARIO_BIG_FORM_TRANSFORM_TIME		600
#define MARIO_RACCOON_FORM_TRANSFORM_TIME	400
#define MARIO_SPEED_TELEPORT				0.02f
#define MARIO_TELEPORT_TIME					300
#define MARIO_KICK_LIMIT_TIME				250
#define MARIO_FALLING_SPEED					0.35f // MARIO JUMPSPEED - Max(dt) * MARIO_GRAVITY
#define MARIO_FLOATING_TIME					250


//Các trạng thái của Mario
#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING				100
#define MARIO_STATE_RUNNING				200
#define MARIO_STATE_JUMPING				300
#define MARIO_STATE_SUPER_JUMPING		400
#define MARIO_STATE_BRAKING				500
#define MARIO_STATE_KICK				600
#define MARIO_STATE_PICK				700
#define MARIO_STATE_DEATH				800
#define MARIO_STATE_STOP				900
#define MARIO_STATE_SQUAT				1000
#define MARIO_STATE_SHOOT_FIREBALL		1100
#define MARIO_STATE_TAILATTACK			1200
#define MARIO_STATE_FLOATING			1300
#define MARIO_STATE_FLYING				1400


//Mario's Animations
#define MARIO_ANI_SMALL_IDLE			0
#define MARIO_ANI_BIG_IDLE				1
#define MARIO_ANI_FIRE_IDLE				2
#define MARIO_ANI_RACCOON_IDLE			3

#define MARIO_ANI_SMALL_WALKING			4
#define MARIO_ANI_BIG_WALKING			5
#define MARIO_ANI_FIRE_WALKING			6
#define MARIO_ANI_RACCOON_WALKING		7

#define MARIO_ANI_SMALL_JUMPING			8
#define MARIO_ANI_BIG_JUMPING			9
#define MARIO_ANI_FIRE_JUMPING			10
#define MARIO_ANI_RACCOON_JUMPING		11

#define MARIO_ANI_SMALL_RUNNING			12
#define MARIO_ANI_BIG_RUNNING			13
#define MARIO_ANI_FIRE_RUNNING			14
#define MARIO_ANI_RACCOON_RUNNING		15

#define MARIO_ANI_SMALL_FULL_RUNNING	16
#define MARIO_ANI_BIG_FULL_RUNNING		17
#define MARIO_ANI_FIRE_FULL_RUNNING		18
#define MARIO_ANI_RACCOON_FULL_RUNNING	19


#define MARIO_ANI_SMALL_LONG_JUMPING	20
#define MARIO_ANI_BIG_LONG_JUMPING		21
#define MARIO_ANI_FIRE_LONG_JUMPING		22
#define MARIO_ANI_RACCOON_LONG_JUMPING	23

#define MARIO_ANI_SMALL_FULL_BRAKING	24
#define MARIO_ANI_BIG_FULL_BRAKING		25
#define MARIO_ANI_FIRE_FULL_BRAKING		26
#define MARIO_ANI_RACCOON_FULL_BRAKING	27

#define MARIO_ANI_SMALL_KICK_SHELL		28
#define MARIO_ANI_BIG_KICK_SHELL		29
#define MARIO_ANI_FIRE_KICK_SHELL		30
#define MARIO_ANI_RACCOON_KICK_SHELL	31

#define MARIO_ANI_SMALL_HOLD_SHELL_IDLE	32
#define MARIO_ANI_BIG_HOLD_SHELL_IDLE	33
#define MARIO_ANI_FIRE_HOLD_SHELL_IDLE	34
#define MARIO_ANI_RACCOON_HOLD_SHELL_IDLE	35

#define MARIO_ANI_SMALL_HOLD_SHELL_RUNNING		36
#define MARIO_ANI_BIG_HOLD_SHELL_RUNNING		37
#define MARIO_ANI_FIRE_HOLD_SHELL_RUNNING		38
#define MARIO_ANI_RACCOON_HOLD_SHELL_RUNNING	39

#define MARIO_ANI_BIG_SQUAT						40
#define MARIO_ANI_FIRE_SQUAT					41
#define MARIO_ANI_RACCOON_SQUAT					42

#define MARIO_ANI_SHOOT_FIRE_BALL				43
#define MARIO_ANI_TAILATTACK					44
#define MARIO_ANI_FLOATING						45
#define MARIO_ANI_FLYING						46
#define MARIO_ANI_DIE							47
#define MARIO_ANI_TURN_TO_BIG_FORM				48
#define MARIO_ANI_TURN_TO_SMALL_FORM			49
#define MARIO_ANI_TURN_TO_RACCOON				50
#define MARIO_ANI_RACCOON_TELEPORT				51
#define MARIO_ANI_HITTED						52
#define MARIO_ANI_LOOKUP						53


#define MARIO_ANI_SMALL_HOLD_SHELL_JUMPING		54
#define MARIO_ANI_BIG_HOLD_SHELL_JUMPING		55
#define MARIO_ANI_FIRE_HOLD_SHELL_JUMPING		56
#define MARIO_ANI_RACCOON_HOLD_SHELL_JUMPING	57


#define MARIO_ANI_SMALL_TELEPORT				58
#define MARIO_ANI_BIG_TELEPORT					59
#define MARIO_ANI_FIRE_TELEPORT					60


#define MARIO_ANI_JUMPING_TAILATTACK			61

#define MARIO_DO_NOTHING						0
#define MARIO_SKILL_FIREBALL					1
#define MARIO_SKILL_TAILATTACK					2



#define FRICTION_TYPE_GROUND					0
#define FRICTION_TYPE_BRICK						1
#define FRICTION_TYPE_MOVING_PLATTFORM			2


#define GROUND_FRICTION							0.0004f
#define BRICK_FRICTION							0.0003f
#define MOVING_PLATTFORM_FRICTION				0.0002f


#define FRICTION								0.0005f
#define JUMPING_FRICTION						0.00005f
// RACOON TAIL
#define RACCOONTAIL_BBOX_WIDTH					9
#define RACCOONTAIL_BBOX_HEIGHT					10


#define RACCOONTAIL_STATE_ATTACK				0
#define RACCOONTAIL_STATE_INACTIVE				1




//GOOMBA
#define GOOMBA_WALKING_SPEED		0.025f;
#define GOOMBA_GRAVITY				0.002f;
#define GOOMBA_DIE_DEFLECT_SPEED_Y	0.4f
#define GOOMBA_DIE_DEFLECT_SPEED_X	0.1f

#define GOOMBA_BBOX_WIDTH			16
#define GOOMBA_BBOX_HEIGHT			15
#define GOOMBA_BBOX_HEIGHT_DIE		9


#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_BEING_STOMPED	300
#define GOOMBA_STATE_BEING_SKILLED	400
#define GOOMBA_STATE_INACTIVE		500
#define GOOMBA_STATE_DEATH			600

#define GOOMBA_ANI_WALKING			0
#define GOOMBA_ANI_DIE				1

#define GOOMA_INACTIVE_TIME			500

//=====================PARA GOOMBA===========
#define PARAGOOMBA_BBOX_WIDTH				20
#define PARAGOOMBA_BBOX_HEIGHT_FLAPPING		24
#define PARAGOOMBA_BBOX_HEIGHT_WALKING		18
#define PARAGOOMBA_GRAVITY					0.001f
#define PARAGOOMBA_WALKING_SPEED			0.025f
#define PARAGOOMBA_JUMPING_STACK			2
#define PARAGOOMBA_JUMP_SPEED				0.15f
#define PARAGOOMBA_SUPER_JUMP_SPEED			0.3f

#define PARAGOOMBA_STATE_WALKING			0
#define PARAGOOMBA_STATE_FLAPPING			1
#define PARAGOOMBA_STATE_SUPER_JUMPING		2
#define PARAGOOMBA_STATE_DEATH				4
#define PARAGOOMBA_STATE_INACTIVE			5
#define PARAGOOMBA_STATE_GOOMBA				6

#define PARAGOOMBA_ANI_WALKING				0
#define PARAGOOMBA_ANI_JUMPING				1

#define PARAGOOMBA_SWITCH_STATE_TIME		2500
#define GOOMA_DEATH_TIME					500

//KoopaTroopa
#define KOOPATROOPA_RED_TYPE				0
#define KOOPATROOPA_GREEN_TYPE				1

#define KOOPATROOPA_WALKING_SPEED				0.03f
#define KOOPATROOPA_GRAVITY						0.002f
#define KOOPATROOPA_BUMPED_GRAVITY				0.012f
#define KOOPATROOPA_BUMP_SPEED					0.145f
#define KOOPATROOPA_FAST_BUMP_SPEED				0.2f
#define KOOPATROOPA_MAX_GRAVITY					0.2f
#define KOOPATROOPA_DIE_DEFLECT_SPEED_Y			0.3f
#define KOOPATROOPA_DIE_DEFLECT_SPEED_X			0.1f
#define KOOPATROPPA_EXIT_HIDING_DEFLECT_SPEED	0.01f
#define KOOPATROOPA_DEFLECT_HOLDING_X			3
#define KOOPATROOPA_DEFLECT_HOLDING_Y			3.5f/10
#define KOOPATROOPA_EXIT_SHELL_TIME				5000
#define KOOPATROOPA_DEATH_TIME					1200
#define KOOPATROOPA_TURN_WALKING_TIME			500

#define KOOPATROOPA_BBOX_WIDTH					16
#define KOOPATROOPA_BBOX_HEIGHT					26
#define KOOPATROOPA_BBOX_HEIGHT_HIDING			16


#define KOOPATROOPA_STATE_WALKING				100
#define KOOPATROOPA_STATE_HIDING				200
#define KOOPATROOPA_STATE_IS_PICKED_UP			300
#define KOOPATROOPA_STATE_BEING_SKILLED			400
#define KOOPATROOPA_STATE_IS_BUMPED				500
#define KOOPATROOPA_STATE_INACTIVE				600
#define KOOPATROOPA_STATE_EXIT_SHELL			700
#define KOOPATROOPA_STATE_DEATH					800


#define KOOPATROOPA_GREEN_ANI_WALKING			0
#define KOOPATROOPA_GREEN_ANI_HIDING			1
#define KOOPATROOPA_GREEN_ANI_BUMPING			2
#define KOOPATROOPA_RED_ANI_WALKING				3
#define KOOPATROOPA_RED_ANI_HIDING				4
#define KOOPATROOPA_RED_ANI_BUMPING				5
#define KOOPATROOPA_GREEN_ANI_EXIT_SHELL		6
#define KOOPATROOPA_RED_ANI_EXIT_SHELL			7
//PARATROOPA
#define PARATROOPA_BBOX_WIDTH			16
#define PARATROOPA_BBOX_HEIGHT			27
#define PARATROOPA_GRAVITY				0.0012f
#define PARATROOPA_WALKING_SPEED		0.05f
#define PARATROOPA_JUMP_SPEED			0.35f

#define PARATROOPA_STATE_JUMPING			0
#define PARATROOPA_STATE_DEATH				1
#define PARATROOPA_STATE_INACTIVE			2
#define PARATROOPA_STATE_KOOPA				3

#define PARATROOPA_ANI_FLAPPING				0
#define PARATROOPA_ANI_DEATH				1

#define PARATROOPA_SWITCH_STATE_TIME		1000
#define PARATROOPA_DEATH_TIME				1000

// RED TROOPA
#define PARATROOPA_STATE_FLYING				0
#define PARATROOPA_FLYING_SPEED				0.05f
#define PARATROOPA_CHANGE_DIRECT_TIME		2000




//PiranhaPlant
#define PIRANHAPLANT_GREEN_TYPE					0
#define PIRANHAPLANT_RED_TYPE					1

#define PIRANHAPLANT_BBOX_WIDTH					16
#define PIRANHAPLANT_BBOX_HEIGHT				31
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_BBOX_DEATH_WIDTH			16
#define PIRANHAPLANT_BBOX_DEATH_HEIGHT			16

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_BITING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_GREEN_ANI_DARTING		0
#define PIRANHAPLANT_GREEN_ANI_BITING		1
#define PIRANHAPLANT_RED_ANI_DARTING		2
#define PIRANHAPLANT_RED_ANI_BITING			3
#define PIRANHAPLANT_ANI_DEATH				4

#define PIRANHAPLANT_SWITCHING_STATE_TIME	1000
#define PIRANHAPLANT_INACTIVE_TIME			800
//Fire PiranhaPlant
//PiranhaPlant
#define FIREPIRANHAPLANT_GREEN_TYPE				0
#define FIREPIRANHAPLANT_RED_TYPE				1

#define FIREPIRANHAPLANT_BBOX_WIDTH				16
#define FIREPIRANHAPLANT_BBOX_HEIGHT			32
#define FIREPIRANHAPLANT_DARTING_SPEED			0.04f
#define FIREPIRANHAPLANT_BBOX_DEATH_WIDTH		16
#define FIREPIRANHAPLANT_BBOX_DEATH_HEIGHT		16
#define FIREPIRANHAPLANT_FREEFIRE_MAX_ZONE_X	132
#define FIREPIRANHAPLANT_FREEFIRE_MIN_ZONE_X	32
#define FIREPIRANHAPLANT_TARGET_X				103
#define FIREPIRAHANPLANT_SHOOTING_X				5


#define FIREPIRANHAPLANT_STATE_DARTING		0
#define FIREPIRANHAPLANT_STATE_SHOOTING		100
#define FIREPIRANHAPLANT_STATE_DEATH		200
#define FIREPIRANHAPLANT_STATE_INACTIVE		300

#define FIREPIRANHAPLANT_GREEN_ANI_DARTING_BOTTOM	0
#define FIREPIRANHAPLANT_GREEN_ANI_DARTING_TOP		1
#define FIREPIRANHAPLANT_GREEN_ANI_SHOOTING_BOTTOM	2
#define FIREPIRANHAPLANT_GREEN_ANI_SHOOTING_TOP		3

#define FIREPIRANHAPLANT_RED_ANI_DARTING_BOTTOM		4
#define FIREPIRANHAPLANT_RED_ANI_DARTING_TOP		5
#define FIREPIRANHAPLANT_RED_ANI_SHOOTING_BOTTOM	6
#define FIREPIRANHAPLANT_RED_ANI_SHOOTING_TOP		7
#define FIREPIRANHAPLANT_ANI_DEATH					8


#define FIREPIRANHAPLANT_INACTIVE_TIME		800
#define FIREPIRANHAPLANT_SHOOTING_TIME		1500
#define FIREPIRANHAPLANT_SWITCH_TIME		3000
//FIREBALL
#define FIREBALL_GRAVITY		0.008f
#define FIREBALL_SPEED_NX		0.15f
#define FIREBALL_SPEED_NY		0.1f
#define FIREBALL_BBOX_HEIGHT	16
#define FIREBALL_BBOX_WIDTH		8
#define FIREBALL_STATE_NORMAL	100
#define FIREBALL_STATE_EXPLODE	200
#define FIREBALL_STATE_INACTIVE	300
#define FIREBALL_ANI_NORMAL		0
#define FIREBALL_ANI_EXPLODE	1
#define FIREBALL_INACTIVE_TIME	200
//Fireball of PiranhaPlant
#define FIREBULLET_SPEED_NX		0.06f
#define FIREBULLET_SPEED_NY_1	0.02f
#define FIREBULLET_SPEED_NY_2	0.04f
#define FIREBULLET_BBOX_HEIGHT	8
#define FIREBULLET_BBOX_WIDTH	8

#define FIREBULLET_DIRECT_1		1
#define FIREBULLET_DIRECT_2		2
#define FIREBULLET_DIRECT_3		3
#define FIREBULLET_DIRECT_4		4

// BOOMERANG BROTHER	
#define BOOMERANG_BROTHER_BBOX_HEIGHT		24
#define BOOMERANG_BROTHER_BBOX_WIDTH		16
#define BOOMERANG_BROTHER_LIMIT_MOVING		48
#define BOOMERANG_BROTHER_LIMIT_THROWING	80


#define BOOMERANG_BROTHER_STATE_WALKING		0
#define BOOMERANG_BROTHER_STATE_THROWING	1
#define BOOMERANG_BROTHER_STATE_INACTIVE	2
#define BOOMERANG_BROTHER_STATE_DEATH		3

#define BOOMERANG_BROTHER_SPEED_X			0.04f
#define BOOMERANG_BROTHER_JUMPING_SPEED		0.3f

#define BOOMERANG_BROTHER_ANI_WALKING		0
#define BOOMERANG_BROTHER_ANI_THROWING		1

#define BOOMERANG_BROTHER_CHANGE_STATE_TIME	200


// BOOMERANG
#define BOOMERANG_BBOX_WIDTH				16
#define BOOMERANG_BBOX_HEIGHT				15

#define BOOMERANG_STATE_THROWING			0
#define BOOMERANG_STATE_INACTIVE			1
#define BOOMERANG_STATE_HOLDING				2

#define BOOMERANG_ANI_THROWING				0
#define BOOMERANG_ANI_HOLDING				1

#define BOOMERANG_SPEED_X					0.1f
#define BOOMERANG_SPEED_Y					0.1f

#define BOOMERANG_LIMIT_X					112
#define BOOMERANG_LIMIT_Y					32








//GROUND
#define GROUND_BBOX_WIDTH		16
#define GROUND_BBOX_HEIGHT		16
//Block
#define BLOCK_BBOX_WIDTH		16
#define BLOCK_BBOX_HEIGHT		16
#define ID_TEX_BBOX				-100
// special texture to draw object bounding box
//Brick
#define BRICK_BBOX_WIDTH		16
#define BRICK_BBOX_HEIGHT		16

#define	BRICK_STATE_NORMAL		0
#define BRICK_STATE_EMPTY		1
#define BRICK_STATE_BREAK		2
#define BRICK_STATE_INACTIVE	4

#define BRICK_ANI_NORMAL		0
#define BRICK_ANI_EMPTY			1

#define BRICK_DEFLECT_SPEED		0.2f
#define BRICK_GRAVITY			0.002f
#define BRICK_CHANGE_TIME		3000

#define BRICK_ITEM_COIN_TYPE		1
#define BRICK_ITEM_EXTRA_LIFE_TYPE	2
#define BRICK_STARTMAN_TYPE			3
#define BRICK_POWER_UP_TYPE			4
#define BRICK_BREAKABLE_TYPE		5
#define BRICK_PSWITCH_TYPE			6
#define BRICK_FIREFLOWER_TYPE		7



//Raccoon Leaf 
#define RACCOONLEAF_BBOX_WIDTH			16
#define RACCOONLEAF_BBOX_HEIGHT			14
#define RACCOONLEAF_SPEED_APPEARANCE_Y	0.3f
#define RACCOONLEAF_GRAVITY				0.001f
#define RACCOONLEAF_SPEED_X				0.03f
#define RACCOONLEAF_SPEED_FALL			0.08f
#define RACCOONLEAF_CHANGE_DIRECT_TIME	500

#define RACCOONLEAF_STATE_APPEARANCE	100
#define RACCOONLEAF_STATE_INACTIVE		200

#define RACCOONLEAF_ANI_NORMAL			0
//MUSHROOM

#define MUSHROOM_TYPE_1UP			1
#define MUSHROOM_TYPE_POWERUP		0


#define MUSHROOM_BBOX_WIDTH			16
#define MUSHROOM_BBOX_HEIGHT		16
#define MUSHROOM_SPEED_APPEARANCE_Y	0.04f
#define MUSHROOM_GRAVITY			0.002f
#define MUSHROOM_SPEED_X			0.045f
#define MUSHROOM_APPEARED_TIME		2000

#define MUSHROOM_STATE_APPEARANCE	100
#define MUSHROOM_STATE_INACTIVE		200
#define MUSHROOM_STATE_WALKING		300

#define MUSHROOM_ANI_POWERUP		0
#define MUSHROOM_ANI_1UP			1
// PSWITCH
#define PSWITCH_BBOX_WIDTH			16
#define PSWITCH_BBOX_HEIGHT			16
#define PSWITCH_PRESSED_BBOX_HEIGHT	7
#define PSWITCH_PRESS_APPEAR_TIME	200
#define PSWITCH_SPEED_APPEAR		0.1f
#define PSWITCH_TRANSFORM_TIME		3000

#define PSWITCH_STATE_APPEAR		0
#define PSWITCH_STATE_PRESSED		1
#define PSWITCH_STATE_INACTIVE		2

#define PSWITCH_ANI_APPEAR			0
#define PSWITCH_ANI_PRESSED			1


// PSWITCH
#define FIREFLOWER_BBOX_WIDTH			16
#define FIREFLOWER_BBOX_HEIGHT			16
#define FIREFLOWER_APPEAR_TIME			200
#define FIREFLOWER_SPEED_APPEAR_Y		0.04f


#define FIREFLOWER_STATE_APPEAR			0
#define FIREFLOWER_STATE_INACTIVE		1

#define FIREFLOWER_ANI_APPEAR				0



//Invisible Brick
#define INVISIBLEBRICK_TYPE_GROUND	0
#define INVISIBLEBRICK_TYPE_BLOCK	1

//COIN
#define COIN_TYPE_1		1
#define COIN_TYPE_2		2
#define COIN_TYPE_3		3

#define COIN_ANI_TYPE_1	0
#define COIN_ANI_TYPE_2	1

#define COIN_STATE_IDLE		0
#define COIN_STATE_INACTIVE	100
#define COIN_STATE_APPEAR	200

#define COIN_BBOX_WIDTH		14
#define COIN_BBOX_HEIGHT	16
#define COIN_SPEED_DELECT_Y	0.3f
#define COIN_GRAIVITY		0.001f

//POINT EFFECT
#define POINT_TYPE_100		0
#define POINT_TYPE_200		1
#define POINT_TYPE_400		2
#define POINT_TYPE_800		3
#define POINT_TYPE_1000		4
#define POINT_TYPE_2000		5
#define POINT_TYPE_4000		6
#define POINT_TYPE_8000		7
#define POINT_TYPE_1UP		8


#define POINT_ANI_100		0
#define POINT_ANI_200		1
#define POINT_ANI_400		2
#define POINT_ANI_800		3
#define POINT_ANI_1000		4
#define POINT_ANI_2000		5
#define POINT_ANI_4000		6
#define POINT_ANI_8000		7
#define POINT_ANI_1UP		8

#define POINT_APPEAR_SPEED	0.04f
#define POINT_APPEAR_TIME	1000

#define POINT_STATE_APPEAR		0
#define POINT_STATE_INACTIVE	1
//Hit Effect
#define HITEFFECT_ANI			0

#define HITEFFECT_APPEAR_TIME	250

#define HITEFFECT_STATE_APPEAR			0
#define HITEFFECT_STATE_INACTIVE		1
// BROKEN BRICK EFFECT
#define SPRITE_BROKENBRICK_EFFECT_ID	50070


#define BROKENBRICK_EFFECT_SPEED_Y1			0.3f
#define BROKENBRICK_EFFECT_SPEED_Y2			0.5f
#define BROKENBRICK_EFFECT_SPEED_X			0.1f
#define BROKENBRICK_EFFECT_STATE_APPEAR		0
#define BROKENBRICK_EFFECT_STATE_INACTIVE	1
#define BROKENBRICK_EFFECT_APPEAR_TIME		500


//PORTAL
#define PORTAL_ANI_ROULETTE						0
#define PORTAL_ANI_PICKED_MUSHROOM				1
#define PORTAL_ANI_PICKED_STARMAN				2
#define PORTAL_ANI_PICKED_FIREFLOWER			3


#define PORTAL_ROULETTE_CHANGE_TIME		500

#define PORTAL_STATE_ROULETTE			0
#define PORTAL_STATE_PICKED				1
#define PORTAL_STATE_CONGRATULATE		2
#define PORTAL_STATE_INACTIVE			3

#define CARD_MUSHROOM					50010
#define CARD_STARMAN					50012
#define CARD_FIREFLOWER					50011

#define PORTAL_PICKED_SPEED			0.08f
#define PORTAL_BBOX_WIDTH			26
#define PORTAL_BBOX_HEIGHT			26

#define SPRITE_CONGRATULATE				50069
#define SPRITE_CARD_MUSHROOM			50010
#define SPRITE_CARD_FLOWER				50011
#define SPRITE_CARD_STARMAN				50012
#define SPRITE_EMPTY_CARD				50009

// MOVING PLATTFORM 
#define MOVING_PLATTFORM_BBOX_WIDTH		48
#define MOVING_PLATTFORM_BBOX_HEIGHT	16

#define MOVING_PLATTFORM_SPEED_X		0.03f
#define MOVING_PLATTFORM_GRAVITY		0.0001f

#define MOVING_PLATTFORM_STATE_MOVING	0
#define MOVING_PLATTFORM_STATE_FALLING	1
#define MOVING_PLATTFORM_STATE_INACTIVE	2

#define MOVING_PLAT_FORM_ANI			0


// MOVING EDGE
#define MOVING_EDGE_SPEED				0.025f
#define MOVING_EDGE_STATE_MOVING		0
#define MOVING_EDGE_STATE_INACTIVE		1			






//Game PLAY 
#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAPS				7
#define SCENE_SECTION_GRID				8
#define BOTTOM_EDGE_WORLD				490
#define BOTTOM_EDGE_EXTRAMAP			756
// HUD
#define FONT_BBOX_WIDTH					8.0f
#define SPRITE_HUD1_ID					50001
#define SPRITE_HUD2_ID					50002
#define SPRITE_POWERMELTER_ID			50007
#define SPRITE_POWEMELTER_ARROW_ID		50008
#define SPRITE_FILLED_POWERMELTER_ID	50005
#define SPRITE_FILLED_POWERMETER_ARROW_ID	50006
#define DEFAULT_TIME					300
#define SPRITE_MARIO_PLAYER_ID			50003
#define SPRITE_LUGI_PLAYER_ID			50004
#define HUD_BBOX_HEIGHT					58


#define HUD_HEIGHT						101
#define HUD_TRANSLATE_Y					-80.0f
#define HUD1_TRANSLATE_X				 28.0f
#define HUD2_TRANSLATE_X				195.0f
#define PLAYER_SPRITE_TRANSLATE_X		32.0f
#define PLAYER_SPRTIE_TRANSPATE_Y		15.0f
#define WORLD_SPRITE_TRANSLATE_Y		7.0f
#define LIFE_SPRITE_TRANSLATE_X			57.0f
#define WORLD_SPRITE_TRANSLATE_X		65.0f
#define CARD_SPRITE_TRANSLATE_Y			2.0f
#define REMAINTIME_SPRITE_TRANSLATE_X	152.0f
#define SCORE_SPRITE_TRANSLATE_X		80.0f
#define MONEY_SPRITE_TRANSLATE_X		160.0f
#define CARD_SPRITE_TRANSLATE_X			195.0f
#define CARD_WIDTH						24.0f
//FONT

#define SPRITE_FONT_0_ID				50013
#define SPRITE_FONT_1_ID				50014
#define SPRITE_FONT_2_ID				50015
#define SPRITE_FONT_3_ID				50016
#define SPRITE_FONT_4_ID				50017
#define SPRITE_FONT_5_ID				50018
#define SPRITE_FONT_6_ID				50019
#define SPRITE_FONT_7_ID				50020
#define SPRITE_FONT_8_ID				50021
#define SPRITE_FONT_9_ID				50022
#define SPRITE_FONT_A_ID				50023
#define SPRITE_FONT_B_ID				50024
#define SPRITE_FONT_C_ID				50025
#define SPRITE_FONT_D_ID				50026
#define SPRITE_FONT_E_ID				50027
#define SPRITE_FONT_F_ID				50028
#define SPRITE_FONT_G_ID				50029
#define SPRITE_FONT_H_ID				50030
#define SPRITE_FONT_I_ID				50031
#define SPRITE_FONT_J_ID				50032
#define SPRITE_FONT_K_ID				50033
#define SPRITE_FONT_L_ID				50034
#define SPRITE_FONT_M_ID				50035
#define SPRITE_FONT_N_ID				50036
#define SPRITE_FONT_O_ID				50037
#define SPRITE_FONT_P_ID				50038
#define SPRITE_FONT_Q_ID				50039
#define SPRITE_FONT_R_ID				50040
#define SPRITE_FONT_S_ID				50041
#define SPRITE_FONT_T_ID				50042
#define SPRITE_FONT_U_ID				50043
#define SPRITE_FONT_V_ID				50044
#define SPRITE_FONT_W_ID				50045
#define SPRITE_FONT_X_ID				50046
#define SPRITE_FONT_Y_ID				50047
#define SPRITE_FONT_Z_ID				50048


// Animation ID 

#define ANIMATION_SET_HUD_ID			77
#define ANIMATION_SET_PORTAL_ID			76
#define ANIMATION_SET_COIN_ID			70
#define ANIMATION_SET_GREEN_PARATROOPA	9
#define ANIMATION_SET_RED_PARATROOPA	10
#define ANIMATION_SET_PARA_GOOMBA		7
#define ANIMATION_SET_KOOPA				4
#define ANIMATION_SET_FIRE_PIRAHAPLANT	6
#define ANIMATION_SET_PIRAHAPLANT		5
#define ANIMATION_SET_BROWN_GOOMBA		3
#define ANIMATION_SET_RED_GOOMBA		8
#define ANIMATION_SET_FIREBALL			60
#define ANIMATION_SET_RACCOONLEAF		71
#define ANIMATION_SET_MUSHROOM			72
#define ANIMATION_SET_PSWITCH			73
#define ANIMATION_SET_POINTEFFECT		74
#define ANIMATION_SET_HIT_EFFECT		75
#define ANIMATION_SET_INVISBLE			99
#define ANIMATION_SET_BRICK				42
#define ANIMATION_SET_QUESTION_BRICK	43
#define ANIMATION_SET_BREAKABLE_BRICK	44
#define ANIMATION_SET_BOOMERANG_BROTHER	11
#define ANIMATION_SET_MARIO				1
#define ANIMATION_SET_LUGI				2
#define ANIMATION_SET_MOVING_PLATTFORM	78
#define ANIMATION_SET_BOOMERANG			61
#define ANIMATION_SET_FIREFLOWER		79

// ANimation Card
#define ANIMATION_TAKEN_MUSHROOM_CARD	859
#define ANIMATION_TAKEN_STAR_CARD		860
#define ANIMATION_TAKEN_FIREFLOWER_CARD	861







// PLAY SCENE
#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_BLOCKS				5
#define OBJECT_TYPE_GROUNDS				4
#define OBJECT_TYPE_PIPES				6
#define OBJECT_TYPE_INVISIBLEBRICK		7
#define OBJECT_TYPE_PIRANHAPLANT		8
#define OBJECT_TYPE_COIN				9
#define OBJECT_TYPE_FIREPIRANHAPLANT	10
#define OBJECT_TYPE_PARAGOOMBA			11
#define OBJECT_TYPE_PARATROOPA			12
#define OBJECT_TYPE_HUD					13
#define OBJECT_TYPE_PORTAL				50
#define OBJECT_TYPE_LUGI				24
#define OBJECT_TYPE_REDPARATROOPA		25
#define OBJECT_TYPE_MOVING_PLATTFORM	26
#define OBJECT_TYPE_BOOMRERANG_BROTHER	27
#define OBJECT_TYPE_MOVING_EDGE			28
#define OBJECT_TYPE_FIREFLOWER			29

#define DONE_SCENE_POINT				10000
#define DONE_SCENE_DELAY_TIME			2000
//WORLD MAP

#define OBJECT_TYPE_BUSH				20
#define OBJECT_TYPE_PANEL				21
#define OBJECT_TYPE_PLAYER				22

#define WORLDMAP_BACKGROUND_SPEED		0.25f
#define POINT_TO_STOP					2.3f

//PANEL 
#define WORLDMAP_PANEL_TYPE_BLOCK			0
#define WORLDMAP_PANEL_TYPE_ACTIONSCENE		1

#define WORLDMAP_PANEL_STATE_NORMAL			0
#define WORLDMAP_PANEL_STATE_CLEARED		1
	

#define WORLDMAP_PANEL_ANI_NORMAL			0
#define WORLDMAP_PANEL_ANI_CLEARED			1
#define WORLDMAP_PANEL_CHANGE_STATE_TIME	1000
#define WORLDMAP_PANEL_ACTIVE_PLAYER_TIME	1500
//WORLDMAP Player
#define WORLD_MAP_PLAYER_ANI_SMALL		0
#define WORLD_MAP_PLAYER_ANI_BIG		1
#define WORLD_MAP_PLAYER_ANI_RACCOON	2
#define WORLD_MAP_PLAYER_ANI_FIRE		3

#define WORLD_MAP_PLAYER_SMALL_HEIGHT	16
#define WORLD_MAP_PLAYER_BIG_HEIGHT		21
#define WORLD_MAP_PLAYER_RACCOON_HEIGHT	24


//INTRO SCENE
#define OBJECT_TYPE_BACKGROUND_INTRO_SCENE	23
#define OBJECT_TYPE_MUSHROOM				25
#define OBJECT_TYPE_RACCOONLEAF				26
#define OBJECT_TYPE_STARMAN					27
#define OBJECT_TYPE_SHELL					28




#define SPRITE_SCROLLING_CURTAIN_ID			40070
#define SPRITE_GROUND_ID					40071
#define SPRITE_BACKGROUND_ID				40072
#define SPRITE_SHINNING3_ID					40074
#define SPRITE_BUSH1_ID						40078
#define SPRITE_BUSH2_ID						40079
#define BUSH2_BBOX_WIDTH					63
#define SCROLLING_CURTAIN_HEIGHT			182
#define BACKGROUND_BBOX_WIDTH				256
#define BACKGROUND_BBOX_HEIGHT				187
#define MENU_BBOX_WIDTH						112


#define SPRITE_MENU_PICKED_PLAYER1_ID		40080
#define SPRITE_MENU_PICKED_PLAYER2_ID		40081

#define SCROLLING_SPEED						0.06f
#define BACKGROUND_SHAKING_SPEED			0.5f
#define SPEED_TO_AUTOFLOATING				0.09f
#define CHANGE_STATE_TIME					200
#define SECTION2_TIME						500
#define SECTION3_TIME						400
#define SECTION3_SHAKING_TIME				1000
#define SECTION3_CHANGE_BACKGROUND_TIME		800

#define INTROSCENE_START_POSITION			16
#define INTROSCENE_END_POSITION				258
#define BEGIN_SECTION3_Y					30

#define OBJECT_TYPE_PLAYER_SPEED		0.2f

#define MAX_SCENE_LINE					1024
//GAME
#define MAX_GAME_LINE					1024
#define GAME_FILE_SECTION_UNKNOWN	    -1
#define GAME_FILE_SECTION_SETTINGS		1
#define GAME_FILE_SECTION_SCENES		2
//Tile Map
#define TILE_WIDTH						16
#define TILE_HEIGHT						16

//Pipe
#define PIPE_NORMAL_TYPE					0
#define PIPE_EXTRAMAP_PORT_TYPE_UP_DOWN		1
#define PIPE_EXTRAMAP_PORT_TYPE_DOWN_UP		2
#define PIPE_EXTRAMAP_PORT_TYPE_UP_UP		3
#define PIPE_EXTRAMAP_PORT_TYPE_DOWN_DOWN	4
#define PIPE_BBOX_WIDTH					30
#define PIPE_BBOX_HEIGHT				16
//SETTING
#define WINDOW_CLASS_NAME L"SuperMarioBros3"
#define MAIN_WINDOW_TITLE L"Super Mario Bros 3"
#define BACKGROUND_COLOR_PLAYSCENE D3DCOLOR_XRGB(181, 236, 243)
#define BACKGROUND_COLOR_SCENE D3DCOLOR_XRGB	(0, 0, 0)
#define SCREEN_WIDTH							320
#define SCREEN_HEIGHT							288
#define MAX_FRAME_RATE							120


//GRID
#define CELL_WIDTH	177
#define CELL_HEIGHT	144

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) \
if(ptr) \
{\
	delete (ptr); \
	ptr = nullptr; \
} \

#endif