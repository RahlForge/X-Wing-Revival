/*
 *  Ship.h
 */

#pragma once
class Ship;
class ShipEngine;

#include "PlatformSpecific.h"

#include "GameObject.h"
#include <string>
#include <map>
#include <cfloat>
#include "ShipClass.h"
#include "Model.h"
#include "Shot.h"
#include "Turret.h"


class Ship : public GameObject
{
public:
	const ShipClass *Class;
	uint8_t Team;
	Model Shape;
	std::string Name;
	bool CanRespawn;
	uint8_t Group;
	bool IsMissionObjective;
	
	double Health;
	Clock HitClock;
	uint32_t HitByID;
	bool HitRear;
	Vec3D CockpitOffset;
	Clock DeathClock;
	double JumpProgress;
	std::map<std::string,double> Subsystems;
	std::vector<Vec3D> SubsystemCenters;     // FIXME: Move these to ShipClass?
	std::vector<std::string> SubsystemNames; //
	double CollisionPotential;
	double ShieldF, ShieldR;
	uint8_t ShieldPos;
	
	bool Firing;
	uint8_t SelectedWeapon;
	std::map<uint8_t,Clock> FiringClocks;
	std::map<uint8_t,int8_t> Ammo;
	std::map<uint8_t,uint8_t> FiringMode;
	uint8_t WeaponIndex;
	uint8_t FiredThisFrame;
	
	uint32_t Target;
	uint8_t TargetSubsystem;
	float TargetLock;
	uint32_t NextCheckpoint;
	
	int8_t EngineSoundDir;
	Clock EngineSoundClock;
	std::string EngineSoundPrev;
	std::vector<ShipEngine> Engines;
	
	
	Ship( uint32_t id = 0 );
	Ship( const ShipClass *ship_class );
	virtual ~Ship();
	
	void Clear( void );
	void ClientInit( void );
	bool SetClass( uint32_t ship_class_id );
	void SetClass( const ShipClass *ship_class );
	void Reset( void );
	
	void SetHealth( double health );
	void AddDamage( double front, double rear, const char *subsystem = NULL, uint32_t hit_by_id = 0, double max_hull_damage = FLT_MAX );
	void KnockCockpit( const Vec3D *dir, double force );
	
	void SetRoll( double roll, double dt );
	void SetPitch( double pitch, double dt );
	void SetYaw( double yaw, double dt );
	double GetThrottle( void ) const;
	void SetThrottle( double throttle, double dt );
	std::string SetThrottleGetSound( double throttle, double dt );
	void SetShieldPos( uint8_t pos );
	
	double Radius( void ) const;
	double MaxSpeed( void ) const;
	double Acceleration( void ) const;
	double MaxGeneric( double slow, double fast, double exponent ) const;
	double MaxRoll( void ) const;
	double MaxPitch( void ) const;
	double MaxYaw( void ) const;
	double MaxRollChange( void ) const;
	double MaxPitchChange( void ) const;
	double MaxYawChange( void ) const;
	double MaxHealth( void ) const;
	double MaxShield( void ) const;
	double ShieldRechargeDelay( void ) const;
	double ShieldRechargeRate( void ) const;
	double ExplosionRate( void ) const;
	double PiecesDangerousTime( void ) const;
	int WeaponCount( int weapon_type ) const;
	uint8_t Category( void ) const;
	bool PlayersCanFly( void ) const;
	
	Pos3D HeadPos( void ) const;
	double Exploded( void ) const;
	int ExplosionSeed( void ) const;
	const char *FlybySound( double speed ) const;
	
	std::map<int,Shot*> NextShots( GameObject *target = NULL, uint8_t firing_mode = 0 ) const;
	std::map<int,Shot*> AllShots( GameObject *target = NULL ) const;
	void JustFired( void );
	void JustFired( uint8_t weapon, uint8_t mode );
	bool NextWeapon( void );
	bool NextFiringMode( void );
	uint8_t CurrentFiringMode( void ) const;
	double ShotDelay( void ) const;
	int8_t AmmoForWeapon( uint8_t weapon = 0 ) const;
	float LockingOn( const GameObject *target ) const;
	void UpdateTarget( const GameObject *target, uint8_t subsystem = 0, double dt = 0. );
	Pos3D TargetCenter( uint8_t subsystem = 0 ) const;
	std::string SubsystemName( uint8_t subsystem ) const;
	uint8_t SubsystemID( std::string subsystem ) const;
	
	void ResetTurrets( void ) const;
	Turret *AttachedTurret( uint8_t index = 0 ) const;
	std::list<Turret*> AttachedTurrets( void ) const;
	std::set<Player*> PlayersInTurrets( void ) const;
	Player *Owner( void ) const;
	
	bool PlayerShouldUpdateServer( void ) const;
	bool ServerShouldUpdatePlayer( void ) const;
	bool ServerShouldUpdateOthers( void ) const;
	bool CanCollideWithOwnType( void ) const;
	bool CanCollideWithOtherTypes( void ) const;
	bool ComplexCollisionDetection( void ) const;
	
	void AddToInitPacket( Packet *packet, int8_t precision = 0 );
	void ReadFromInitPacket( Packet *packet, int8_t precision = 0 );
	void AddToUpdatePacketFromServer( Packet *packet, int8_t precision = 0 );
	void ReadFromUpdatePacketFromServer( Packet *packet, int8_t precision = 0 );
	void AddToUpdatePacketFromClient( Packet *packet, int8_t precision = 0 );
	void ReadFromUpdatePacketFromClient( Packet *packet, int8_t precision = 0 );
	
	bool WillCollide( const GameObject *other, double dt, std::string *this_object = NULL, std::string *other_object = NULL ) const;
	bool WillCollideWithSphere( const GameObject *other, double other_radius, double dt, std::string *this_object ) const;
	void Update( double dt );
	
	double DrawOffset( void ) const;
	double CockpitDrawOffset( void ) const;
	
	void Draw( void );
	void DrawWireframe( Color *color = NULL, double scale = 1. );
	
	std::map<ShipEngine*,Pos3D> EnginePositions( void );
	std::map<ShipEngine*,Pos3D> EnginePositions( const Pos3D *pos );
	
	enum
	{
		SHIELD_CENTER = 0,
		SHIELD_FRONT,
		SHIELD_REAR
	};
};


class ShipEngine : public Vec3D
{
public:
	Animation Texture;
	double Radius;
	Color DrawColor;
	
	ShipEngine( const ShipClassEngine *engine );
	ShipEngine &operator = ( const ShipEngine &other );
	
	void DrawAt( const Pos3D *pos, float alpha = 1.f );
};
