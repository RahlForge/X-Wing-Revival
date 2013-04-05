/*
 *  LobbyMenu.cpp
 */

#include "LobbyMenu.h"

#include "XWingDefs.h"
#include "XWingGame.h"
#include "Num.h"


LobbyMenu::LobbyMenu( void )
{
	Rect.x = 0;
	Rect.w = 0;
	Rect.w = Raptor::Game->Gfx.W;
	Rect.h = Raptor::Game->Gfx.H;
	
	Background.BecomeInstance( Raptor::Game->Res.GetAnimation("bg_lobby.ani") );
	
	bool tiny = (Raptor::Game->Gfx.H < 600) || (Raptor::Game->Gfx.W < 800);
	TitleFont = Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 24 : 30 );
	
	AddElement( LeaveButton = new LobbyMenuLeaveButton( this ) );
	AddElement( FlyButton = new LobbyMenuFlyButton( this ) );
	AddElement( TeamButton = new LobbyMenuTeamButton( this ) );
	AddElement( ShipButton = new LobbyMenuShipButton( this ) );
	
	AddElement( PlayerList = new ListBox( this, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 15 : 24 ), 16 ) );
	PlayerList->SelectedRed = PlayerList->TextRed;
	PlayerList->SelectedGreen = PlayerList->TextGreen;
	PlayerList->SelectedBlue = PlayerList->TextBlue;
	PlayerList->SelectedAlpha = PlayerList->TextAlpha;
	
	AddElement( MessageList = new ListBox( this, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 15 : 17 ), 16 ) );
	MessageList->SelectedRed = MessageList->TextRed;
	MessageList->SelectedGreen = MessageList->TextGreen;
	MessageList->SelectedBlue = MessageList->TextBlue;
	MessageList->SelectedAlpha = MessageList->TextAlpha;
	
	AddElement( PlayerName = new TextBox( this, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 15 : 24 ), Font::ALIGN_MIDDLE_LEFT ) );
	PlayerName->ReturnDeselects = false;
	PlayerName->EscDeselects = true;
	PlayerName->PassReturn = true;
	PlayerName->PassEsc = true;
	PlayerName->SelectedRed = 1.f;
	PlayerName->SelectedGreen = 1.f;
	PlayerName->SelectedBlue = 0.f;
	PlayerName->SelectedAlpha = 1.f;
	
	AddElement( MessageInput = new TextBox( this, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 15 : 17 ), Font::ALIGN_TOP_LEFT ) );
	MessageInput->ReturnDeselects = false;
	MessageInput->EscDeselects = false;
	MessageInput->PassReturn = true;
	MessageInput->PassEsc = true;
	MessageInput->TextRed = 1.f;
	MessageInput->TextGreen = 1.f;
	MessageInput->TextBlue = 1.f;
	MessageInput->TextAlpha = 0.5f;
	MessageInput->SelectedTextRed = 1.f;
	MessageInput->SelectedTextGreen = 1.f;
	MessageInput->SelectedTextBlue = 0.f;
	MessageInput->SelectedTextAlpha = 1.f;
	MessageInput->Red = MessageList->Red;
	MessageInput->Green = MessageList->Green;
	MessageInput->Blue = MessageList->Blue;
	MessageInput->Alpha = MessageList->Alpha;
	MessageInput->SelectedRed = MessageInput->Red;
	MessageInput->SelectedGreen = MessageInput->Green;
	MessageInput->SelectedBlue = MessageInput->Blue;
	MessageInput->SelectedAlpha = MessageInput->Alpha;
	
	AddElement( GameType = new LobbyMenuConfiguration( this, "gametype", "Game Type", tiny ) );
	AddElement( TDMKillLimit = new LobbyMenuConfiguration( this, "tdm_kill_limit", "Team Kill Limit", tiny ) );
	AddElement( DMKillLimit = new LobbyMenuConfiguration( this, "dm_kill_limit", "Kill Limit", tiny ) );
	AddElement( AI = new LobbyMenuConfiguration( this, "ai_waves", "AI Ships", tiny ) );
	AddElement( Respawn = new LobbyMenuConfiguration( this, "respawn", "Respawn", tiny ) );
	AddElement( Asteroids = new LobbyMenuConfiguration( this, "asteroids", "Asteroids", tiny ) );
	AddElement( Permissions = new LobbyMenuConfiguration( this, "permissions", "Game Settings", tiny ) );
	
	UpdateRects();
}


LobbyMenu::~LobbyMenu()
{
}


void LobbyMenu::UpdateRects( void )
{
	Rect.x = 0;
	Rect.w = 0;
	Rect.w = Raptor::Game->Gfx.W;
	Rect.h = Raptor::Game->Gfx.H;
	
	LeaveButton->Rect.h = LeaveButton->LabelFont->GetHeight() + 4;
	FlyButton->Rect.h = LeaveButton->Rect.h;
	
	LeaveButton->Rect.x = 32;
	
	if( Rect.w >= 1024 )
		LeaveButton->Rect.w = 384;
	else
		LeaveButton->Rect.w = Rect.w / 2 - TeamButton->Rect.w / 2 - LeaveButton->Rect.x * 2;
	FlyButton->Rect.w = LeaveButton->Rect.w;
	
	LeaveButton->Rect.y = Rect.h - LeaveButton->Rect.h - 32;
	FlyButton->Rect.y = LeaveButton->Rect.y;
	
	FlyButton->Rect.x = Rect.w - FlyButton->Rect.w - LeaveButton->Rect.x;
	
	PlayerName->Rect.x = LeaveButton->Rect.x;
	PlayerName->Rect.w = (FlyButton->Rect.x + FlyButton->Rect.w - PlayerList->Rect.x) / 2 - 5;
	PlayerName->Rect.y = TitleFont->GetHeight() + 20;
	
	TeamButton->Rect.x = LeaveButton->Rect.x;
	TeamButton->Rect.y = PlayerName->Rect.y + PlayerName->Rect.h + 10;
	TeamButton->Rect.w = 140;
	TeamButton->Rect.h = TeamButton->LabelFont->GetHeight() + 6;
	
	ShipButton->Rect.x = TeamButton->Rect.x + TeamButton->Rect.w + 10;
	ShipButton->Rect.y = TeamButton->Rect.y;
	ShipButton->Rect.w = TeamButton->Rect.w;
	ShipButton->Rect.h = TeamButton->Rect.h;
	
	ShipButton->Enabled = true;
	Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
	if( player )
	{
		bool ffa = ( Raptor::Game->Data.Properties["gametype"].find("ffa_") == 0 );
		if( (player->Properties["team"] == "Spectator") || ((! ffa) && (player->Properties["team"] == "")) )
			ShipButton->Enabled = false;
	}
	
	if( ShipButton->Enabled )
	{
		ShipButton->Alpha = TeamButton->Alpha;
		ShipButton->AlphaNormal = TeamButton->AlphaNormal;
		ShipButton->RedOver = TeamButton->RedOver;
		ShipButton->BlueOver = TeamButton->BlueOver;
		ShipButton->GreenOver = TeamButton->GreenOver;
		ShipButton->AlphaOver = TeamButton->AlphaOver;
	}
	else
	{
		ShipButton->Alpha = 0.5f;
		ShipButton->AlphaNormal = 0.5f;
		ShipButton->RedOver = TeamButton->RedNormal;
		ShipButton->BlueOver = TeamButton->BlueNormal;
		ShipButton->GreenOver = TeamButton->GreenNormal;
		ShipButton->AlphaOver = 0.5f;
	}
	
	PlayerList->Rect.x = LeaveButton->Rect.x;
	PlayerList->Rect.w = (FlyButton->Rect.x + FlyButton->Rect.w - PlayerList->Rect.x) / 2 - 5;
	PlayerList->Rect.y = TeamButton->Rect.y + TeamButton->Rect.h + 10;
	PlayerList->Rect.h = 400 - (PlayerName->Rect.h + TeamButton->Rect.h + 20);
	if( PlayerList->Rect.h > Rect.h / 3 )
		PlayerList->Rect.h = Rect.h / 3;
	
	MessageInput->Rect.x = PlayerList->Rect.x;
	MessageInput->Rect.w = Rect.w - (MessageList->Rect.x * 2);
	MessageInput->Rect.h = MessageInput->TextFont->GetHeight();
	MessageInput->Rect.y = FlyButton->Rect.y - MessageInput->Rect.h - MessageInput->Rect.x;
	
	MessageList->Rect.x = MessageInput->Rect.x;
	MessageList->Rect.w = MessageInput->Rect.w;
	MessageList->Rect.y = PlayerList->Rect.y + PlayerList->Rect.h + MessageList->Rect.x;
	MessageList->Rect.h = MessageInput->Rect.y - (PlayerList->Rect.y + PlayerList->Rect.h) - MessageList->Rect.x;
	
	if( MessageList->Rect.h * 2 > PlayerList->Rect.h )
	{
		int h = MessageList->Rect.h + PlayerList->Rect.h;
		MessageList->Rect.h = h / 3;
		PlayerList->Rect.h = h - MessageList->Rect.h;
		MessageList->Rect.y = PlayerList->Rect.y + PlayerList->Rect.h + MessageList->Rect.x;
	}
	
	Permissions->Rect.x = PlayerList->Rect.x + PlayerList->Rect.w + 10;
	Permissions->Rect.y = PlayerName->Rect.y;
	Permissions->Rect.w = PlayerList->Rect.w;
	Permissions->Update();
	LobbyMenuConfiguration *prev = Permissions;
	
	GameType->Rect.x = prev->Rect.x;
	GameType->Rect.y = prev->Rect.y + prev->Rect.h + 10;
	GameType->Rect.w = prev->Rect.w;
	GameType->Update();
	prev = GameType;
	
	if( Asteroids->Enabled )
	{
		Asteroids->Rect.x = prev->Rect.x;
		Asteroids->Rect.y = prev->Rect.y + prev->Rect.h + 10;
		Asteroids->Rect.w = prev->Rect.w;
		Asteroids->Update();
		prev = Asteroids;
	}
	
	if( AI->Enabled )
	{
		AI->Rect.x = prev->Rect.x;
		AI->Rect.y = prev->Rect.y + prev->Rect.h + 10;
		AI->Rect.w = prev->Rect.w;
		AI->Update();
		prev = AI;
	}
	
	if( TDMKillLimit->Enabled )
	{
		TDMKillLimit->Rect.x = prev->Rect.x;
		TDMKillLimit->Rect.y = prev->Rect.y + prev->Rect.h + 10;
		TDMKillLimit->Rect.w = prev->Rect.w;
		TDMKillLimit->Update();
		prev = TDMKillLimit;
	}
	
	if( DMKillLimit->Enabled )
	{
		DMKillLimit->Rect.x = prev->Rect.x;
		DMKillLimit->Rect.y = prev->Rect.y + prev->Rect.h + 10;
		DMKillLimit->Rect.w = prev->Rect.w;
		DMKillLimit->Update();
		prev = DMKillLimit;
	}
	
	if( Respawn->Enabled )
	{
		Respawn->Rect.x = prev->Rect.x;
		Respawn->Rect.y = prev->Rect.y + prev->Rect.h + 10;
		Respawn->Rect.w = prev->Rect.w;
		Respawn->Update();
		prev = Respawn;
	}

	if( prev->Rect.y + prev->Rect.h >= MessageList->Rect.y )
	{
		MessageList->Rect.w = PlayerList->Rect.w;
		MessageInput->Rect.w = MessageList->Rect.w;
	}
	
	if( Raptor::Game->Console.IsActive() )
		Selected = NULL;
	else if( PlayerName->IsSelected() )
		;
	else
		Selected = MessageInput;
}


void LobbyMenu::UpdatePlayerName( void )
{
	if( ! PlayerName->IsSelected() )
	{
		Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
		if( player )
			PlayerName->Text = player->Name;
	}
}


void LobbyMenu::UpdatePlayerList( void )
{
	std::string prev_selected = PlayerList->SelectedValue();
	PlayerList->Clear();
	
	bool ffa = ( Raptor::Game->Data.Properties["gametype"].find("ffa_") == 0 );
	
	for( std::map<uint16_t,Player*>::iterator player_iter = Raptor::Game->Data.Players.begin(); player_iter != Raptor::Game->Data.Players.end(); player_iter ++ )
	{
		std::string display_name = player_iter->second->Name;
		
		std::string team = player_iter->second->Properties["team"];
		if( (! ffa) || (team == "Spectator") )
		{
			if( team.empty() )
				display_name += " [Auto-Team]";
			else
				display_name += " [" + team + "]";
		}
		
		if( team != "Spectator" )
		{
			std::string ship = player_iter->second->Properties["ship"];
			if( ! ship.empty() )
				display_name += " [" + ship + "]";
		}
		
		PlayerList->AddItem( Num::ToString( player_iter->second->ID ), display_name );
	}
	
	PlayerList->Select( prev_selected );
}


void LobbyMenu::UpdateMessageList( void )
{
	size_t msg_count = Raptor::Game->Msg.Messages.size();
	if( msg_count != MessageList->Items.size() )
	{
		std::string prev_selected = MessageList->SelectedValue();
		MessageList->Clear();
		
		for( size_t i = 0; i < msg_count; i ++ )
			MessageList->AddItem( Num::ToString((int)i), Raptor::Game->Msg.Messages.at(i)->Text );
		
		MessageList->Select( prev_selected );
		
		MessageList->Scroll = MessageList->MaxScroll();
	}
}


void LobbyMenu::UpdateInfoBoxes( void )
{
	std::string gametype = Raptor::Game->Data.Properties["gametype"];
	if( gametype == "team_elim" )
		GameType->Value->LabelText = "Team Elimination";
	else if( gametype == "ffa_elim" )
		GameType->Value->LabelText = "FFA Elimination";
	else if( gametype == "team_dm" )
		GameType->Value->LabelText = "Team Deathmatch";
	else if( gametype == "ffa_dm" )
		GameType->Value->LabelText = "FFA Deathmatch";
	else if( gametype == "yavin" )
		GameType->Value->LabelText = "Battle of Yavin";
	else
		GameType->Value->LabelText = gametype;
	
	int tdm_kill_limit = atoi( Raptor::Game->Data.Properties["tdm_kill_limit"].c_str() );
	TDMKillLimit->Value->LabelText = Num::ToString(tdm_kill_limit);
	
	int dm_kill_limit = atoi( Raptor::Game->Data.Properties["dm_kill_limit"].c_str() );
	DMKillLimit->Value->LabelText = Num::ToString(dm_kill_limit);
	
	int ai_waves = atoi( Raptor::Game->Data.Properties["ai_waves"].c_str() );
	if( ai_waves <= 0 )
		AI->Value->LabelText = "None";
	else if( ai_waves == 1 )
		AI->Value->LabelText = "Very Few";
	else if( ai_waves == 2 )
		AI->Value->LabelText = "Few";
	else if( ai_waves == 3 )
		AI->Value->LabelText = "Some";
	else if( ai_waves == 4 )
		AI->Value->LabelText = "Many";
	else if( ai_waves == 5 )
		AI->Value->LabelText = "Very Many";
	else if( ai_waves == 6 )
		AI->Value->LabelText = "Holy Crap, Tons of Them!";
	else if( ai_waves == 7 )
		AI->Value->LabelText = "Borderline Ridiculous";
	else if( ai_waves == 8 )
		AI->Value->LabelText = "Undoubtedly Ridiculous";
	else if( ai_waves == 9 )
		AI->Value->LabelText = "Beyond Ridiculous";
	else
		AI->Value->LabelText = "The Whole Friggin' Empire";
	
	std::string respawn = Raptor::Game->Data.Properties["respawn"];
	if( gametype == "team_elim" )
	{
		Respawn->Title->LabelText = "Dead Players";
		Respawn->TitleShadow->LabelText = "Dead Players";
		
		if( respawn == "true" )
			Respawn->Value->LabelText = "Control AI Ships";
		else if( respawn == "false" )
			Respawn->Value->LabelText = "Observe Only";
		else
			Respawn->Value->LabelText = respawn;
	}
	else
	{
		Respawn->Title->LabelText = "Respawn";
		Respawn->TitleShadow->LabelText = "Respawn";
		
		if( respawn == "true" )
			Respawn->Value->LabelText = "Yes";
		else if( respawn == "false" )
			Respawn->Value->LabelText = "No";
		else
			Respawn->Value->LabelText = respawn;
	}
	
	int asteroids = atoi( Raptor::Game->Data.Properties["asteroids"].c_str() );
	if( asteroids <= 0 )
		Asteroids->Value->LabelText = "None";
	else if( asteroids <= 16 )
		Asteroids->Value->LabelText = "Very Few";
	else if( asteroids <= 32 )
		Asteroids->Value->LabelText = "Few";
	else if( asteroids <= 64 )
		Asteroids->Value->LabelText = "Some";
	else if( asteroids <= 128 )
		Asteroids->Value->LabelText = "Many";
	else if( asteroids <= 256 )
		Asteroids->Value->LabelText = "Very Many";
	else if( asteroids <= 512 )
		Asteroids->Value->LabelText = "Uncomfortably Many";
	else if( asteroids <= 1024 )
		Asteroids->Value->LabelText = "Insanely Many";
	else if( asteroids <= 2048 )
		Asteroids->Value->LabelText = "Hardly Worth Playing";
	else
		Asteroids->Value->LabelText = "Not Worth Playing";
	
	std::string permissions = Raptor::Game->Data.Properties["permissions"];
	if( permissions == "all" )
		Permissions->Value->LabelText = "Anyone Can Change";
	else
		Permissions->Value->LabelText = "Only Host Can Change";
	
	// Determine player permissions.
	Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
	bool admin = (player && player->Properties["admin"] == "true");
	bool permissions_all = (Raptor::Game->Data.Properties["permissions"] == "all");
	bool flying = Raptor::Game->Data.GameObjects.size();
	
	// Show "Fly" button for admin or late joiners.
	FlyButton->Enabled = (admin || permissions_all || flying);
	
	// Show "Change" buttons for admin.
	FlyButton->Visible = FlyButton->Enabled;
	GameType->ShowButton = (admin || permissions_all) && (! flying);
	TDMKillLimit->ShowButton = (admin || permissions_all) && (! flying);
	DMKillLimit->ShowButton = (admin || permissions_all) && (! flying);
	AI->ShowButton = (admin || permissions_all) && (! flying);
	Respawn->ShowButton = (admin || permissions_all) && (! flying);
	Asteroids->ShowButton = (admin || permissions_all) && (! flying);
	Permissions->ShowButton = admin;
	
	// Hide "Respawn" unless it's Team Elimination or Yavin.
	if( (gametype != "team_elim") && (gametype != "yavin") )
	{
		Respawn->ShowButton = false;
		Respawn->Visible = false;
		Respawn->Enabled = false;
	}
	else
	{
		Respawn->Visible = true;
		Respawn->Enabled = true;
	}
	
	// Hide "Team Kill Limit" unless it's Team Deathmatch.
	if( gametype != "team_dm" )
	{
		TDMKillLimit->ShowButton = false;
		TDMKillLimit->Visible = false;
		TDMKillLimit->Enabled = false;
	}
	else
	{
		TDMKillLimit->Visible = true;
		TDMKillLimit->Enabled = true;
	}
	
	// Hide "Kill Limit" unless it's FFA Deathmatch.
	if( gametype != "ffa_dm" )
	{
		DMKillLimit->ShowButton = false;
		DMKillLimit->Visible = false;
		DMKillLimit->Enabled = false;
	}
	else
	{
		DMKillLimit->Visible = true;
		DMKillLimit->Enabled = true;
	}
	
	// Hide "Asteroids" if Battle of Yavin.
	if( gametype == "yavin" )
	{
		Asteroids->ShowButton = false;
		Asteroids->Visible = false;
		Asteroids->Enabled = false;
	}
	else
	{
		Asteroids->Visible = true;
		Asteroids->Enabled = true;
	}
}


void LobbyMenu::Draw( void )
{
	UpdateInfoBoxes();
	UpdatePlayerName();
	UpdatePlayerList();
	UpdateMessageList();
	UpdateRects();
	
	Raptor::Game->Gfx.DrawRect2D( Rect.w / 2 - Rect.h, 0, Rect.w / 2 + Rect.h, Rect.h, Background.CurrentFrame(), 1.f, 1.f, 1.f, 1.f );
	
	Layer::Draw();
	
	std::string title = "Pre-Game Lobby";
	if( Raptor::Game->Data.GameObjects.size() )
		title = "Game-In-Progress Lobby";
	
	TitleFont->DrawText( title, Rect.w/2 + 2, 12, Font::ALIGN_TOP_CENTER, 0.f,0.f,0.f,0.8f );
	TitleFont->DrawText( title, Rect.w/2, 10, Font::ALIGN_TOP_CENTER );
}


bool LobbyMenu::KeyDown( SDLKey key )
{
	if( MessageInput->IsSelected() )
	{
		if( (key == SDLK_RETURN) || (key == SDLK_KP_ENTER) )
		{
			std::string msg = MessageInput->Text;
			MessageInput->Text = "";
			
			if( ! msg.empty() )
			{
				Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
				
				Packet message = Packet( Raptor::Packet::MESSAGE );
				message.AddString( ((player ? player->Name : std::string("Anonymous")) + std::string(":  ") + msg).c_str() );
				message.AddUInt( TextConsole::MSG_CHAT );
				Raptor::Game->Net.Send( &message );
			}
			
			return true;
		}
	}
	else if( PlayerName->IsSelected() )
	{
		if( (key == SDLK_RETURN) || (key == SDLK_KP_ENTER) )
		{
			if( PlayerName->Text.size() )
			{
				Raptor::Game->Cfg.Settings[ "name" ] = PlayerName->Text;
				
				Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
				if( player )
				{
					player->Name = PlayerName->Text;
					
					Packet player_properties = Packet( Raptor::Packet::PLAYER_PROPERTIES );
					player_properties.AddUShort( Raptor::Game->PlayerID );
					player_properties.AddUInt( 1 );
					player_properties.AddString( "name" );
					player_properties.AddString( player->Name );
					Raptor::Game->Net.Send( &player_properties );
				}
			}
			else
				UpdatePlayerName();
			
			Selected = MessageInput;
			return true;
		}
		else if( key == SDLK_ESCAPE )
		{
			UpdatePlayerName();
			Selected = MessageInput;
			return true;
		}
	}
	
	// Return false if the event was not handled.
	return false;
}


// ---------------------------------------------------------------------------


LobbyMenuFlyButton::LobbyMenuFlyButton( LobbyMenu *menu )
: LabelledButton( menu, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", 48 ), "Fly", Font::ALIGN_MIDDLE_CENTER, Raptor::Game->Res.GetAnimation("button.ani"), Raptor::Game->Res.GetAnimation("button_mdown.ani") )
{
	Red = 1.f;
	Green = 1.f;
	Blue = 1.f;
	Alpha = 0.75f;
}


void LobbyMenuFlyButton::Clicked( Uint8 button )
{
	if( !( Enabled && Visible ) )
		return;
	
	if( button != SDL_BUTTON_LEFT )
		return;
	
	Packet fly = Packet( XWing::Packet::FLY );
	Raptor::Game->Net.Send( &fly );
}


// ---------------------------------------------------------------------------


LobbyMenuLeaveButton::LobbyMenuLeaveButton( LobbyMenu *menu )
: LabelledButton( menu, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", 48 ), "Leave", Font::ALIGN_MIDDLE_CENTER, Raptor::Game->Res.GetAnimation("button.ani"), Raptor::Game->Res.GetAnimation("button_mdown.ani") )
{
	Red = 1.f;
	Green = 1.f;
	Blue = 1.f;
	Alpha = 0.75f;
}


void LobbyMenuLeaveButton::Clicked( Uint8 button )
{
	if( button != SDL_BUTTON_LEFT )
		return;
	
	Raptor::Game->Net.DisconnectNice();
}


// ---------------------------------------------------------------------------


LobbyMenuTeamButton::LobbyMenuTeamButton( LobbyMenu *menu )
: LabelledButton( menu, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", 18 ), "Change Team", Font::ALIGN_MIDDLE_CENTER, Raptor::Game->Res.GetAnimation("button.ani"), Raptor::Game->Res.GetAnimation("button_mdown.ani") )
{
	Red = 1.f;
	Green = 1.f;
	Blue = 1.f;
	Alpha = 1.f;
}


void LobbyMenuTeamButton::Clicked( Uint8 button )
{
	Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
	if( ! player )
		return;
	
	std::string team = player->Properties["team"];
	
	bool ffa = ( Raptor::Game->Data.Properties["gametype"].find("ffa_") == 0 );
	
	if( ! ffa )
	{
		bool go_prev = ((button == SDL_BUTTON_RIGHT) || (button == SDL_BUTTON_WHEELUP));
		
		if( player && (player->Properties["team"] == "Rebel") )
			team = go_prev ? "" : "Empire";
		else if( player && (player->Properties["team"] == "Empire") )
			team = go_prev ? "Rebel" : "Spectator";
		else if( player && (player->Properties["team"] == "Spectator") )
			team = go_prev ? "Empire" : "";
		else if( player )
			team = go_prev ? "Spectator" : "Rebel";
	}
	else
	{
		if( team == "Spectator" )
			team = "";
		else
			team = "Spectator";
	}
	
	std::string ship = "";
	if( team == "Rebel" )
		ship = "X/W";
	else if( team == "Empire" )
		ship = "T/F";
	
	Packet player_properties = Packet( Raptor::Packet::PLAYER_PROPERTIES );
	player_properties.AddUShort( Raptor::Game->PlayerID );
	player_properties.AddUInt( 2 );
	player_properties.AddString( "team" );
	player_properties.AddString( team );
	player_properties.AddString( "ship" );
	player_properties.AddString( ship );
	Raptor::Game->Net.Send( &player_properties );
}


// ---------------------------------------------------------------------------


LobbyMenuShipButton::LobbyMenuShipButton( LobbyMenu *menu )
: LabelledButton( menu, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", 18 ), "Change Ship", Font::ALIGN_MIDDLE_CENTER, Raptor::Game->Res.GetAnimation("button.ani"), Raptor::Game->Res.GetAnimation("button_mdown.ani") )
{
	Red = 1.f;
	Green = 1.f;
	Blue = 1.f;
	Alpha = 1.f;
}


void LobbyMenuShipButton::Clicked( Uint8 button )
{
	Player *player = Raptor::Game->Data.GetPlayer( Raptor::Game->PlayerID );
	if( ! player )
		return;
	
	std::string team = player->Properties["team"];
	
	if( team != "Spectator" )
	{
		bool ffa = ( Raptor::Game->Data.Properties["gametype"].find("ffa_") == 0 );
		
		std::string ship = "";
		
		if( ffa )
		{
			team = "";
			
			bool go_prev = ((button == SDL_BUTTON_RIGHT) || (button == SDL_BUTTON_WHEELUP));
			
			if( player->Properties["ship"] == "X/W" )
				ship = go_prev ? "T/F" : "Y/W";
			else if( player->Properties["ship"] == "Y/W" )
				ship = go_prev ? "X/W" : "T/F";
			else
				ship = go_prev ? "Y/W" : "X/W";
		}
		else if( player->Properties["team"] == "Rebel" )
		{
			if( player->Properties["ship"] == "X/W" )
				ship = "Y/W";
			else
				ship = "X/W";
		}
		else if( player->Properties["team"] == "Empire" )
		{
			ship = "T/F";
		}
		
		Packet player_properties = Packet( Raptor::Packet::PLAYER_PROPERTIES );
		player_properties.AddUShort( Raptor::Game->PlayerID );
		player_properties.AddUInt( 2 );
		player_properties.AddString( "team" );
		player_properties.AddString( team );
		player_properties.AddString( "ship" );
		player_properties.AddString( ship );
		Raptor::Game->Net.Send( &player_properties );
	}
}


// ---------------------------------------------------------------------------


LobbyMenuConfiguration::LobbyMenuConfiguration( LobbyMenu *menu, std::string property, std::string desc, bool tiny )
: Layer( menu )
{
	Property = property;
	
	AddElement( TitleShadow = new Label( this, &Rect, desc, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 12 : 18 ), Font::ALIGN_TOP_CENTER ) );
	AddElement( Title = new Label( this, &Rect, desc, TitleShadow->LabelFont, TitleShadow->LabelAlign ) );
	
	AddElement( ValueShadow = new Label( this, &Rect, "", Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 14 : 24 ), Font::ALIGN_TOP_CENTER ) );
	AddElement( Value = new Label( this, &Rect, "", ValueShadow->LabelFont, ValueShadow->LabelAlign ) );
	
	AddElement( ChangeButton = new LobbyMenuConfigChangeButton( this, tiny ) );
	ShowButton = false;
	
	TitleShadow->Red = 0.f;
	TitleShadow->Green = 0.f;
	TitleShadow->Blue = 0.f;
	TitleShadow->Alpha = 0.8f;
	
	ValueShadow->Red = TitleShadow->Red;
	ValueShadow->Green = TitleShadow->Green;
	ValueShadow->Blue = TitleShadow->Blue;
	ValueShadow->Alpha = TitleShadow->Alpha;
	
	Update();
}


void LobbyMenuConfiguration::Update( void )
{
	Title->Rect.w = Rect.w;
	Title->Rect.h = Title->LabelFont->GetHeight();
	Title->Rect.x = 0;
	Title->Rect.y = 0;
	
	TitleShadow->LabelText = Title->LabelText;
	TitleShadow->Rect.w = Title->Rect.w;
	TitleShadow->Rect.h = Title->Rect.h;
	TitleShadow->Rect.x = Title->Rect.x + 2;
	TitleShadow->Rect.y = Title->Rect.y + 2;
	
	Value->Rect.w = Rect.w;
	Value->Rect.h = Title->LabelFont->GetHeight();
	Value->Rect.x = 0;
	Value->Rect.y = Title->Rect.y + Title->Rect.h + 1;
	
	ValueShadow->LabelText = Value->LabelText;
	ValueShadow->Rect.w = Value->Rect.w;
	ValueShadow->Rect.h = Value->Rect.h;
	ValueShadow->Rect.x = Value->Rect.x + 2;
	ValueShadow->Rect.y = Value->Rect.y + 2;
	
	ChangeButton->Rect.w = (ShowButton && Enabled) ? 100 : 0;
	ChangeButton->Rect.h = (ShowButton && Enabled) ? ChangeButton->LabelFont->GetHeight() + 4 : 0;
	ChangeButton->Rect.x = (Rect.w - ChangeButton->Rect.w) / 2;
	ChangeButton->Rect.y = Value->Rect.y + Value->Rect.h + 7;
	ChangeButton->Enabled = (ShowButton && Enabled);
	ChangeButton->Visible = (ShowButton && Enabled);
	
	Rect.h = ChangeButton->Rect.y + ChangeButton->LabelFont->GetHeight() + 4;
}


// ---------------------------------------------------------------------------


LobbyMenuConfigChangeButton::LobbyMenuConfigChangeButton( LobbyMenuConfiguration *config, bool tiny )
: LabelledButton( config, NULL, Raptor::Game->Res.GetFont( "TimesNR.ttf", tiny ? 12 : 16 ), "Change", Font::ALIGN_MIDDLE_CENTER, Raptor::Game->Res.GetAnimation("button.ani"), Raptor::Game->Res.GetAnimation("button_mdown.ani") )
{
}


void LobbyMenuConfigChangeButton::Clicked( Uint8 button )
{
	LobbyMenuConfiguration *config = (LobbyMenuConfiguration*) Container;
	
	if( !( Enabled && Visible && config->Enabled && config->Visible && config->ShowButton ) )
		return;
	
	std::string value = Raptor::Game->Data.Properties[ config->Property ];
	
	bool go_prev = ((button == SDL_BUTTON_RIGHT) || (button == SDL_BUTTON_WHEELUP));
	
	if( config->Property == "gametype" )
	{
		if( value == "team_elim" )
			value = go_prev ? "yavin" : "team_dm";
		else if( value == "team_dm" )
			value = go_prev ? "team_elim" : "ffa_elim";
		else if( value == "ffa_elim" )
			value = go_prev ? "team_dm" : "ffa_dm";
		else if( value == "ffa_dm" )
			value = go_prev ? "ffa_elim" : "yavin";
		else
			value = go_prev ? "ffa_dm" : "team_elim";
	}
	
	else if( (config->Property == "tdm_kill_limit") || (config->Property == "dm_kill_limit") )
	{
		int new_kill_limit = atoi( value.c_str() ) + (go_prev ? -10 : 10);
		
		if( new_kill_limit == 0 )
			new_kill_limit = 5;
		else if( new_kill_limit < 0 )
			new_kill_limit = 1;
		else if( new_kill_limit == 11 )
			new_kill_limit = 5;
		else if( new_kill_limit == 15 )
			new_kill_limit = 10;
		
		value = Num::ToString( new_kill_limit );
	}
	
	else if( config->Property == "ai_waves" )
	{
		int new_ai_waves = atoi( value.c_str() ) + (go_prev ? -1 : 1);
		
		if( new_ai_waves > 10 )
			new_ai_waves = 0;
		if( new_ai_waves < 0 )
			new_ai_waves = 10;
		
		value = Num::ToString( new_ai_waves );
	}
	
	else if( config->Property == "respawn" )
	{
		if( value == "false" )
			value = "true";
		else
			value = "false";
	}
	
	else if( config->Property == "asteroids" )
	{
		int new_asteroids = atoi( value.c_str() );
		if( ! go_prev )
		{
			new_asteroids *= 2;
			if( new_asteroids > 4096 )
				new_asteroids = 0;
			else if( ! new_asteroids )
				new_asteroids = 16;
		}
		else
		{
			if( new_asteroids == 0 )
				new_asteroids = 4096;
			else if( new_asteroids <= 16 )
				new_asteroids = 0;
			else
				new_asteroids /= 2;
		}
		
		value = Num::ToString( new_asteroids );
	}
	
	else if( config->Property == "permissions" )
	{
		if( value == "all" )
			value = "admin";
		else
			value = "all";
	}
	
	Packet info = Packet( Raptor::Packet::INFO );
	info.AddUShort( 1 );
	info.AddString( config->Property );
	info.AddString( value );
	Raptor::Game->Net.Send( &info );
}
