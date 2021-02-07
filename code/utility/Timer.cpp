/*# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#*/
#include "Timer.hpp"

namespace utility
{
    Timer::Timer()
    {
        //Initialize the variables
        m_start_ticks = 0;
        m_paused_ticks = 0;

        m_paused = false;
        m_started = false;
    }

    void Timer::start()
    {
        //Start the timer
        m_started = true;

        //Unpause the timer
        m_paused = false;

        //Get the current clock time
        m_start_ticks = SDL_GetTicks();
        m_paused_ticks = 0;
    }

    void Timer::stop()
    {
        //Stop the timer
        m_started = false;

        //Unpause the timer
        m_paused = false;

        //Clear tick variables
        m_start_ticks = 0;
        m_paused_ticks = 0;
    }

    void Timer::pause()
    {
        //If the timer is running and isn't already paused
        if( m_started && !m_paused )
        {
            //Pause the timer
            m_paused = true;

            //Calculate the paused ticks
            m_paused_ticks = SDL_GetTicks() - m_start_ticks;
            m_start_ticks = 0;
        }
    }

    void Timer::unpause()
    {
        //If the timer is running and paused
        if( m_started && m_paused )
        {
            //Unpause the timer
            m_paused = false;

            //Reset the starting ticks
            m_start_ticks = SDL_GetTicks() - m_paused_ticks;

            //Reset the paused ticks
            m_paused_ticks = 0;
        }
    }

    uint32_t Timer::getTicks()
    {
        //The actual timer time
        uint32_t time = 0;

        //If the timer is running
        if( m_started )
        {
            //If the timer is paused
            if( m_paused )
            {
                //Return the number of ticks when the timer was paused
                time = m_paused_ticks;
            }
            else
            {
                //Return the current time minus the start time
                time = SDL_GetTicks() - m_start_ticks;
            }
        }

        return time;
    }

    bool Timer::isStarted()
    {
        //Timer is running and paused or unpaused
        return m_started;
    }

    bool Timer::isPaused()
    {
        //Timer is running and paused
        return m_paused && m_started;
    }
}
