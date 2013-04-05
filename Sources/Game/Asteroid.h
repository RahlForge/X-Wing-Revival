/*
 *  Asteroid.h
 */

#pragma once
class Asteroid;

#include "platforms.h"

#include "GameObject.h"
#include "Model.h"
#include "Animation.h"
#include "Color.h"


class Asteroid : public GameObject
{
public:
	double Radius;
	Model *Shape;
	Animation Texture;
	Color Ambient, Diffuse;
	
	Asteroid( uint32_t id = 0 );
	virtual ~Asteroid();
	
	void ClientInit( void );
	
	bool PlayerShouldUpdateServer( void ) const;
	bool ServerShouldUpdatePlayer( void ) const;
	bool ServerShouldUpdateOthers( void ) const;
	bool CanCollideWithOwnType( void ) const;
	bool CanCollideWithOtherTypes( void ) const;
	bool IsMoving( void ) const;
	
	void AddToInitPacket( Packet *packet, int8_t precision = 0 );
	void ReadFromInitPacket( Packet *packet, int8_t precision = 0 );
	
	bool WillCollide( const GameObject *other, double dt ) const;
	
	void Draw( void );
};
