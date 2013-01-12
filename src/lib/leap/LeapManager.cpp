#include "LeapManager.h"

#ifdef ENABLE_LEAPMOTION

#include <common/Debug.h>
#include <iostream>

namespace funk
{

static const int LEAP_NUM_FRAMES_CACHE = 16;

void LeapListener::Init( LeapManager * owner )
{
	m_owner = owner;
}

void LeapListener::onInit(const Leap::Controller& controller) 
{
	printf("Leap Initialized\n");
}

void LeapListener::onConnect(const Leap::Controller& controller) 
{
	printf("Leap Connected\n");
}

void LeapListener::onDisconnect(const Leap::Controller& controller) 
{
	printf("Leap Disconnected\n");
}

void LeapListener::onFrame(const Leap::Controller& controller) 
{
	m_owner->OnFrame(controller);
}

LeapManager::LeapManager()
{
	Init();
}

LeapManager::~LeapManager()
{
}


void LeapManager::Init()
{
	m_currFrame = 0;
	m_frames.resize(LEAP_NUM_FRAMES_CACHE);

	m_listener.Init(this);
	m_ctrl = Leap::Controller(&m_listener);
}

void LeapManager::OnFrame( const Leap::Controller& ctrl)
{
	const Leap::Frame frameRaw = ctrl.frame();
	const std::vector<Leap::Hand>& hands = frameRaw.hands();
	const size_t numHands = hands.size();
	printf("Frameid: %d, timestamp: %d, hands: %d\n", (int)frameRaw.id(), (int)frameRaw.timestamp(), (int)numHands );

	LeapFrame & frame = m_frames[ (m_currFrame+1) % m_frames.size() ];

	frame.id = (unsigned int)frameRaw.id();
	frame.timestamp =  (unsigned int)frameRaw.timestamp();
	frame.hands.clear();
	frame.hands.resize( hands.size() );

	const float coeffScale = 1.0f/250.0f;

	// int hands
	for( size_t i = 0; i < hands.size(); ++i )
	{
		const Leap::Hand & handRaw = hands[i];

		LeapHand & hand = frame.hands[i];

		// handball
		if ( handRaw.ball() )
		{
			hand.handball.pos = v3( (float)handRaw.ball()->position.x, (float)handRaw.ball()->position.y, (float)handRaw.ball()->position.z ) * coeffScale;
			hand.handball.radius = (float)handRaw.ball()->radius * coeffScale;
			hand.handball.valid = true;
		}
		else 
		{
			hand.handball.valid = false;
		}

		// init fingers
		hand.fingers.resize( handRaw.fingers().size() );
		for( size_t j = 0; j < handRaw.fingers().size(); ++j )
		{
			const Leap::Finger & fingerRaw = handRaw.fingers()[j];

			LeapFinger & finger = hand.fingers[j];
			finger.pos = v3( (float)fingerRaw.tip().position.x,  (float)fingerRaw.tip().position.y,  (float)fingerRaw.tip().position.z ) * coeffScale;
			finger.dir = -v3(  (float)fingerRaw.tip().direction.x,  (float)fingerRaw.tip().direction.y,  (float)fingerRaw.tip().direction.z );
		}
	}

	// flip frame
	m_currFrame = (m_currFrame+1) % m_frames.size();
}

}

#endif