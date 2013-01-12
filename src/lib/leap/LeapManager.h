#ifndef _INCLUDE_LEAP_MANAGER_H
#define _INCLUDE_LEAP_MANAGER_H

#define ENABLE_LEAPMOTION
#ifdef ENABLE_LEAPMOTION

#include "Leap.h"

#include <common/Singleton.h>
#include <common/StrongHandle.h>
#include <math/v3.h>

#include <vector>

namespace funk
{
	class LeapManager;

	class LeapListener : public Leap::Listener
	{
	private: 
		LeapManager * m_owner;

	public:

		void Init( LeapManager * owner );

		// callbacks from the Leap API
		virtual void onInit(const Leap::Controller&);
		virtual void onConnect(const Leap::Controller&);
		virtual void onDisconnect(const Leap::Controller&);
		virtual void onFrame(const Leap::Controller&);
	};

	struct LeapFinger
	{
		funk::v3 pos;
		funk::v3 dir;
	};

	struct LeapHandball
	{
		funk::v3 pos;
		float radius;
		bool valid;
	};

	struct LeapHand
	{
		std::vector<LeapFinger> fingers;
		LeapHandball handball;		
	};

	struct LeapFrame
	{
		std::vector<LeapHand> hands;
		unsigned int id;
		unsigned int timestamp;
	};

	class LeapManager : public Singleton<LeapManager>, public Leap::Listener
	{
	private:
		LeapListener m_listener;
		Leap::Controller m_ctrl;

		// frames
		int m_currFrame;
		std::vector<LeapFrame> m_frames;

	public:

		void OnFrame( const Leap::Controller& ctrl);

		const LeapFrame &GetCurrFrame() const { return m_frames[m_currFrame]; }

	private:
		friend Singleton<LeapManager>;
		LeapManager();
		~LeapManager();

		void Init();

	};
}

#endif

#endif