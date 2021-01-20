#include "SoundManager.h"
#include <iostream>


/*****************************
* Clase que representar un par sonido-canal
* Hasheable, con posibilidad de usar como clave el sonido o el canal
* Un par entre sonido y canmal tiene sentido porque los sonidos necesitan un canal propio para reproducirse
* Lo que me da la posibilidad de encontrar sonidos dado el canal, y canales dado el sonido.
******************************/
class SoundChannelPair : public Hasheable
{
    int m_key;
public:
    SoundChannelPair(FMOD_CHANNEL * channel, FMOD_SOUND * sound, float volume, bool repeat, int dest, int paramKey = 0)
            : Channel(channel), Sound(sound), Volume(volume), Repeat(repeat), Dest(dest)
    {
        if (!paramKey)                   // por default el canal es la key
            m_key = (int)channel;
        else
            m_key = (int)sound;
    }
    FMOD_CHANNEL * Channel;         // La clase es privada, asi que me doy este lujete
    FMOD_SOUND * Sound;             // No los dejo como const porque me puede interesar cambiarlos
    float Volume;                   // Me habia olvidado! Si viene con un volumen distinto al default al repetir se caga todo jaja
    bool Repeat;
    int Dest;
    int getHashCode()
    {
        return m_key;
    }
};


/// Globales ...
/// Meto variables de esta forma por un tema de performance
/// Si las dejo como miembros tengo que dejar visible la clase SoundChannelPair
/// Y si la dejo visible debo declarar las varables privadas
/// El asunto es que anda mas rapido el callback si esta todo aca

static HashTable      *  g_hashByChannel; /// Par sonido-canal por canal
static HashTable      *  g_hashBySound;   /// Par sonido-canal por sonido
static FMOD_SYSTEM    *  g_fmodSystem;    /// Sistema principal de fmod
static FMOD_CAPS         g_fmod_Caps;
static FMOD_RESULT       g_fmodResult;
static FMOD_SPEAKERMODE  g_fmodSpeakermode;
static unsigned int      g_fmodVersion;
static bool              g_InitOK;
static vector<FMOD_CHANNEL*> g_musicSounds;
static vector<FMOD_CHANNEL*> g_gameSounds;
/// Los sonidos se van guardando en un mapa a medida que van siendo cargados
map<string, FMOD_SOUND*> g_sounds;

static bool ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cout<<"ERROR : FMOD error ("<<result<<") "<<FMOD_ErrorString(result)<<std::endl;
        return false;
    }
    return true;
}

FMOD_RESULT F_CALLBACK finDelPlay(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, int command, unsigned int commanddata1, unsigned int commanddata2);
static void internalPlay(FMOD_SOUND* sound, float volume, bool repeat, int dest) // 0 para musica, 1 para otros
{
    FMOD_CHANNEL *channel = 0;
    g_fmodResult = FMOD_System_PlaySound(g_fmodSystem, FMOD_CHANNEL_FREE, sound, false, &channel);
    if (!ERRCHECK(g_fmodResult)){
        return;
    }

    g_fmodResult = FMOD_Channel_SetVolume(channel, volume);
    if (!ERRCHECK(g_fmodResult))
        return;

    g_fmodResult = FMOD_Channel_SetCallback(channel, FMOD_CHANNEL_CALLBACKTYPE_END, finDelPlay, 0);
    if (!ERRCHECK(g_fmodResult))
        return;

    if (!dest)
        g_musicSounds.push_back(channel);
    else
        g_gameSounds.push_back(channel);
    g_hashByChannel->add(new SoundChannelPair(channel, sound, volume, repeat, dest)); //channel es la key
    g_hashBySound->add(new SoundChannelPair(channel, sound, volume, repeat, dest, 1)); //sound es la key
}

static void removeChannel(FMOD_CHANNEL *channel, vector<FMOD_CHANNEL *> & vect)
{
    for (int i = 0; i < vect.size(); i++)
        if (vect[i] == channel)
            vect.erase(vect.begin() + i);
}

/// CallBack que se dispara al cuando termina un sonido
/// Los callbacks se disparan solo si se llama a la funcion update
/// Por eso debe haber una llamada a SoundManager::update() por tic!
/// Este callback solo sirve para los sonidos que se repiten
FMOD_RESULT F_CALLBACK finDelPlay(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, int command, unsigned int commanddata1, unsigned int commanddata2)
{
    // key por canal
    int cKey = (int)channel;
    if (SoundChannelPair* scp = dynamic_cast<SoundChannelPair*>(g_hashByChannel->remove(cKey)))
    {
        FMOD_SOUND* s = scp->Sound;
        int sKey = (int)s;
        if (scp->Repeat){
            internalPlay(s, scp->Volume, scp->Repeat, scp->Dest);
        }
        if (!scp->Dest){
            removeChannel(channel, g_musicSounds);
        }else{
            removeChannel(channel, g_gameSounds);
        }
        delete ( g_hashBySound->remove(sKey) );
        delete scp;
    }
    return FMOD_OK;
}

SoundManager* SoundManager::s_instance = NULL;

SoundManager* SoundManager::getInstance()
{
    if (!s_instance)
        s_instance = new SoundManager();
    return s_instance;
}

bool SoundManager::Init()
{
    g_fmodResult = FMOD_System_Create(&g_fmodSystem);
    if (!ERRCHECK(g_fmodResult)) return false;

    g_fmodResult = FMOD_System_GetVersion(g_fmodSystem, &g_fmodVersion);
    if (!ERRCHECK(g_fmodResult)) return false;
    if (g_fmodVersion < FMOD_VERSION)
    {
        std::cout<<"ERROR  : Estas usando una version vieja de fmod, version de fmod requerida: "<<FMOD_VERSION<<std::endl;
        std::cout<<"WARNING: No habra sonido en el juego"<<std::endl;
        return false;
    }

    g_fmodResult = FMOD_System_GetDriverCaps(g_fmodSystem,0, &g_fmod_Caps, 0, 0, &g_fmodSpeakermode);
    if (!ERRCHECK(g_fmodResult)) return false;

    g_fmodResult = FMOD_System_SetSpeakerMode(g_fmodSystem, g_fmodSpeakermode);       /* Set the user selected speaker mode. */
    if (!ERRCHECK(g_fmodResult)) return false;

    if (g_fmod_Caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
    {
        std::cout<<"WARNING: No hay aceleracion de sonido, muy malo para la latencia"<<std::endl;     /* You might want to warn the user about this. */
        g_fmodResult = FMOD_System_SetDSPBufferSize(g_fmodSystem, 1024, 10);    /* At 48khz, the latency between issuing an fmod command and hearing it will now be about 213ms. */
        if (!ERRCHECK(g_fmodResult)) return false;
    }

    g_fmodResult = FMOD_System_Init(g_fmodSystem, 100, FMOD_INIT_NORMAL, 0);    /* Replace with whatever channel count and flags you use! */
    if (g_fmodResult == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        g_fmodResult = FMOD_System_SetSpeakerMode(g_fmodSystem, FMOD_SPEAKERMODE_STEREO);
        if (!ERRCHECK(g_fmodResult)) return false;

        g_fmodResult = FMOD_System_Init(g_fmodSystem, 100, FMOD_INIT_NORMAL, 0); /* Replace with whatever channel count and flags you use! */
        if (!ERRCHECK(g_fmodResult)) return false;
    }
    g_hashByChannel = new HashTable();
    g_hashBySound = new HashTable();
    //g_fmodSystem = m_fmodSystem;
    return true;
}


SoundManager::SoundManager()
{
    m_musicVol = 0.7;
    m_gameVol = 0.7;
    g_InitOK = Init();
}

SoundManager::~SoundManager()
{
    for ( map<string, FMOD_SOUND*>::iterator it = g_sounds.begin(); it != g_sounds.end(); it++)
    {
        FMOD_Sound_Release(it->second);
    }
    FMOD_System_Close(g_fmodSystem);
    FMOD_System_Release(g_fmodSystem);
}


FMOD_SOUND* SoundManager::getSound(string fileName)
{
    FMOD_SOUND *sound = NULL;
    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it == g_sounds.end())
    {
        if (fileName.find_first_of('\\') == string::npos && fileName.find_first_of('/') == string::npos)
            fileName = "./Sonidos/" + fileName;

        g_fmodResult = FMOD_System_CreateSound(g_fmodSystem, fileName.data(), FMOD_DEFAULT | FMOD_3D, 0, &sound);
        if (!ERRCHECK(g_fmodResult)){
            LOG_MORE(LogLevel::Error,"Could not create sound: %s %s",FMOD_ErrorString(g_fmodResult), fileName.data());
            return NULL;
        }
        g_sounds.insert(std::make_pair(fileName, sound));
    }
    else
    {// tengo el archivo de sonido cargado
        sound = it->second;
    }
    return sound;
}


void SoundManager::Play(const string & fileName, bool repeat, float volume)
{
    if (!g_InitOK) return;

    FMOD_SOUND *sound = getSound(fileName);
    if (sound)
    {
        g_sounds.insert(std::make_pair(fileName, sound));
        internalPlay(sound, volume, repeat, 1);
    }
}


void SoundManager::Stop(const string & fileName)
{
    if (!g_InitOK) return;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)it->second)))
        {
            FMOD_BOOL isPlaying = 0;
            g_fmodResult = FMOD_Channel_IsPlaying(scp->Channel, &isPlaying);
            if (!ERRCHECK(g_fmodResult)) return;
            if (!isPlaying){
                g_fmodResult = FMOD_Channel_GetPaused(scp->Channel, &isPlaying);
                if (!ERRCHECK(g_fmodResult)) return;
            }
            if (isPlaying)
            {
                delete ( g_hashByChannel->remove((int)scp->Channel) ); // lo saco antes para que no tenga efecto el callback
                g_hashBySound->remove((int)scp->Sound);
                if (!scp->Dest){
                    removeChannel(scp->Channel, g_musicSounds);
                }else{
                    removeChannel(scp->Channel, g_gameSounds);
                }
                g_fmodResult = FMOD_Channel_Stop(scp->Channel);
                if (!ERRCHECK(g_fmodResult)) return;
            }
            delete scp;
        }
    }
}

bool SoundManager::isPlaying (const string & fileName)
{
    if (!g_InitOK) return false;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)(it->second))))
        {
            SoundChannelPair *scp = (SoundChannelPair *)g_hashBySound->getValue((int)(it->second));
            FMOD_BOOL ret = 0;
            g_fmodResult = FMOD_Channel_IsPlaying(scp->Channel, &ret);
            if (!ERRCHECK(g_fmodResult)) return false;
            return ret;
        }
    }
    return false;
}

void SoundManager::update()
{
    if (!g_InitOK) return;

    FMOD_System_Update(g_fmodSystem);
}

void SoundManager::Pause(const string & fileName)
{
    if (!g_InitOK) return;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)it->second)))
        {
            FMOD_BOOL isPlaying = 0;
            g_fmodResult = FMOD_Channel_IsPlaying(scp->Channel, &isPlaying);
            if (!ERRCHECK(g_fmodResult)) return;
            if (isPlaying)
            {
                g_fmodResult =  FMOD_Channel_SetPaused(scp->Channel, 1);
                if (!ERRCHECK(g_fmodResult)) return;
            }
        }
    }
}

void SoundManager::Resume(const string & fileName)
{
    if (!g_InitOK) return;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)it->second)))
        {
            FMOD_BOOL isPaused = 0;
            g_fmodResult = FMOD_Channel_GetPaused(scp->Channel, &isPaused);
            if (!ERRCHECK(g_fmodResult)) return;
            if (isPaused)
            {
                g_fmodResult =  FMOD_Channel_SetPaused(scp->Channel, 0);
                if (!ERRCHECK(g_fmodResult)) return;
            }
        }
    }
}


bool SoundManager::isPaused(const string & fileName)
{
    if (!g_InitOK) return false;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)(it->second))))
        {
            SoundChannelPair *scp = (SoundChannelPair *)g_hashBySound->getValue((int)(it->second));
            FMOD_BOOL ret = 0;
            g_fmodResult = FMOD_Channel_GetPaused(scp->Channel, &ret);
            if (!ERRCHECK(g_fmodResult)) return false;
            return ret;
        }
    }
    return false;
}

SoundState SoundManager::GetSoundState(const string & fileName)
{
    if (!g_InitOK) return ST_NONE;

    map<string, FMOD_SOUND*>::iterator it = g_sounds.find(fileName);
    if ( it != g_sounds.end())
    {
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)(it->second))))
        {
            SoundChannelPair *scp = (SoundChannelPair *)g_hashBySound->getValue((int)(it->second));

            FMOD_BOOL ret = 0;

            g_fmodResult = FMOD_Channel_GetPaused(scp->Channel, &ret);
            if (!ERRCHECK(g_fmodResult)) return ST_NONE;
            if (ret)
                return ST_PAUSED;

            g_fmodResult = FMOD_Channel_IsPlaying(scp->Channel, &ret);
            if (!ERRCHECK(g_fmodResult)) return ST_NONE;
            if (ret)
                return ST_PLAYING;
        }
    }
    return ST_NONE;
}


void SoundManager::setGameVolume(float volume)
{
    if (!g_InitOK || volume == m_gameVol)
        return;
    m_gameVol = volume;

    for (int i = 0; i < g_gameSounds.size(); i++)
    {
        FMOD_Channel_SetVolume(g_gameSounds[i], volume);
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashByChannel->getValue((int)g_gameSounds[i])))
        {
            scp->Volume = volume;
            scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)scp->Sound));
            scp->Volume = volume;
        }
    }
}

void SoundManager::setMusicVolume(float volume)
{
    if (!g_InitOK || volume == m_musicVol)
        return;
    m_musicVol = volume;
    for (int i = 0; i < g_musicSounds.size(); i++)
    {
        FMOD_Channel_SetVolume(g_musicSounds[i], volume);
        if (SoundChannelPair *scp = dynamic_cast<SoundChannelPair*>(g_hashByChannel->getValue((int)g_musicSounds[i])))
        {
            scp->Volume = volume;
            scp = dynamic_cast<SoundChannelPair*>(g_hashBySound->getValue((int)scp->Sound));
            scp->Volume = volume;
        }
    }
}

void SoundManager::PlayMusic(const string & fileName, bool repeat)
{
    if (!g_InitOK) return;

    FMOD_SOUND *sound = getSound(fileName);
    if (sound)
    {
        g_sounds.insert(std::make_pair(fileName, sound));
        internalPlay(sound, m_musicVol, repeat, 0);
    }
}

void SoundManager::PlayGameSound(const string & fileName, bool repeat)
{
    if (!g_InitOK) return;

    FMOD_SOUND *sound = getSound(fileName);
    if (sound)
    {
        g_sounds.insert(std::make_pair(fileName, sound));
        internalPlay(sound, m_gameVol, repeat, 1);
    }
}
