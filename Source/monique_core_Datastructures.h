#ifndef MONOSYNTH_DATA_H_INCLUDED
#define MONOSYNTH_DATA_H_INCLUDED

#include "App_h_includer.h"

#define FACTORY_NAME "FACTORY DEFAULT (SCRATCH)"

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
enum DATA_TYPES
{
    MORPH = 1,
    MASTER,

    DATA_COPY,

    FILTER_INPUT_ENV_ID_OFFSET = 100,
    EQ_ENV_ID_OFFSET = 200,
    CHORUS_ENV_ID_OFFSET = 300,
};

//==============================================================================
enum WAVE_TYPES
{
    SINE,
    SAW,
    SQUARE,
    NOICE
};

//==============================================================================
enum FILTER_TYPS
{
    LPF_2_PASS = 1,
    HPF,
    BPF,
    HIGH_2_PASS,
    PASS,
    LPF,
    MOOG_AND_LPF, // TODO OBSOLETE BUT USED IN SOME OLD PROJECTS
    UNKNOWN
};

//==============================================================================
enum MONIQUE_SETUP
{
    SUM_OSCS = 3,
    SUM_FILTERS = 3,
    SUM_INPUTS_PER_FILTER = SUM_OSCS,
    SUM_LFOS = SUM_FILTERS,
    SUM_ENVS = SUM_FILTERS + 1,
    MAIN_ENV = 3,

    LEFT = 0,
    RIGHT = 1,

    SUM_ENV_ARP_STEPS = 16,

    OSC_1 = 0,
    MASTER_OSC = OSC_1,
    OSC_2 = 1,
    OSC_3 = 2,

    FILTER_1 = 0,
    FILTER_2 = 1,
    FILTER_3 = 2,

    SUM_MORPHER_GROUPS = 4,

    SUM_EQ_BANDS = 7
};

#define MIN_CUTOFF 35.0f
#define MAX_CUTOFF 7965.0f

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class MoniqueAudioProcessor;
class DataBuffer // DEFINITION IN SYNTH.CPP
{
    int size;

public:
    // ==============================================================================
    // WORKERS
    // TODO REDUCE TO NEEDED
    mono_AudioSampleBuffer<SUM_EQ_BANDS> band_env_buffers;
    mono_AudioSampleBuffer<SUM_EQ_BANDS> band_out_buffers;

    mono_AudioSampleBuffer<SUM_FILTERS> lfo_amplitudes;
    mono_AudioSampleBuffer<SUM_MORPHER_GROUPS> mfo_amplitudes;
    mono_AudioSampleBuffer<SUM_FILTERS*2> filter_output_samples_l_r;
    mono_AudioSampleBuffer<2> filter_stereo_output_samples;

    mono_AudioSampleBuffer<SUM_OSCS> osc_samples;
    mono_AudioSampleBuffer<1> osc_switchs;
    mono_AudioSampleBuffer<1> osc_sync_switchs;
    mono_AudioSampleBuffer<1> modulator_samples;

    mono_AudioSampleBuffer<1> final_env;
    mono_AudioSampleBuffer<1> chorus_env;

    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_samples;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_input_env_amps;
    mono_AudioSampleBuffer<SUM_INPUTS_PER_FILTER*SUM_FILTERS> filter_output_samples;
    mono_AudioSampleBuffer<SUM_FILTERS> filter_env_amps;

    mono_AudioSampleBuffer<1> tmp_buffer;
    mono_AudioSampleBuffer<1> second_mono_buffer;

private:
    // ==============================================================================
    friend class MoniqueAudioProcessor;
    COLD void resize_buffer_if_required( int size_ ) noexcept;

public:
    // ==============================================================================
    COLD DataBuffer( int init_buffer_size_ ) noexcept;
    COLD ~DataBuffer() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DataBuffer)
};

//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer;
class RuntimeListener
{
    RuntimeNotifyer*const notifyer;
protected:
    //==========================================================================
    double sample_rate;
    float sample_rate_1ths;
    int block_size;

private:
    //==========================================================================
    friend class RuntimeNotifyer;
    COLD virtual void set_sample_rate( double sr_ ) noexcept;
public:
    inline double get_sample_rate() const noexcept {
        return sample_rate;
    }
private:
    COLD virtual void set_block_size( int bs_ ) noexcept;
    COLD virtual void sample_rate_changed( double /* old_sr_ */ ) noexcept;
    COLD virtual void block_size_changed() noexcept;

protected:
    //==========================================================================
    COLD RuntimeListener( RuntimeNotifyer*const notifyer_ ) noexcept;
    COLD ~RuntimeListener() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeListener)
};

//==============================================================================
//==============================================================================
//==============================================================================
class RuntimeNotifyer
{
    //==========================================================================
    friend class RuntimeListener;
    Array<RuntimeListener*> listeners;

    //==========================================================================
    double sample_rate;
    float sample_rate_1ths;
    int block_size;

public:
    //==========================================================================
    void set_sample_rate( double sr_ ) noexcept;
    void set_block_size( int bs_ ) noexcept;

    double get_sample_rate() const noexcept
    {
        return sample_rate;
    }
    int get_block_size() const noexcept
    {
        return block_size;
    }

private:
    //==========================================================================
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< RuntimeNotifyer >;
    COLD RuntimeNotifyer() noexcept;
    COLD ~RuntimeNotifyer() noexcept;
};

//==============================================================================
//==============================================================================
//==============================================================================
struct RuntimeInfo
{
    int64 samples_since_start;
    double bpm;

#ifdef IS_STANDALONE
    bool is_extern_synced;
    bool is_running;

    class ClockCounter
    {
        int clock_counter;
        int clock_counter_absolut;

    public:
        inline void operator++(int) noexcept
        {
            if( ++clock_counter >= 96 )
            {
                clock_counter = 0;
            }
            if( ++clock_counter_absolut >= 96*16 )
            {
                clock_counter_absolut = 0;
            }
        }
        inline int clock() noexcept
        {
            return clock_counter;
        }
        inline int clock_absolut() noexcept
        {
            return clock_counter_absolut;
        }
        inline void reset() noexcept
        {
            clock_counter = 0;
            clock_counter_absolut = clock_counter;
        }

        COLD ClockCounter() : clock_counter(0), clock_counter_absolut(0) {}
    } clock_counter;

    struct Step
    {
        const int step_id;
        const int64 at_absolute_sample;
        const int samples_per_step;

        inline Step( int step_id_, int64 at_absolute_sample_, int64 samples_per_step_ ) noexcept
:
        step_id( step_id_ ),
                 at_absolute_sample( at_absolute_sample_ ),
                 samples_per_step( samples_per_step_ )
        {}
        inline ~Step() noexcept {}
    };
    OwnedArray<Step> steps_in_block;
#endif

private:
    //==========================================================================
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< RuntimeInfo >;
    COLD RuntimeInfo() noexcept;
    COLD ~RuntimeInfo() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RuntimeInfo)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class MorphGroup;
class SmoothedParameter;
class SmoothManager : public RuntimeListener, DeletedAtShutdown
{
    friend class SmoothedParameter;
    Array< SmoothedParameter* > smoothers;
    RuntimeNotifyer*const notifyer;

    //==========================================================================
    friend class MoniqueSynthData;
    friend class ContainerDeletePolicy< SmoothManager >;
    COLD SmoothManager(RuntimeNotifyer*const notifyer_) noexcept;
    COLD ~SmoothManager() noexcept;

public:
    void smooth( int num_samples_, int glide_motor_time_in_ms_ ) noexcept;
    void automated_morph( const float* morph_amount_, int num_samples_, int morph_motor_time_in_ms_, MorphGroup*morph_group_ ) noexcept;
    void reset() noexcept;

public:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothManager)
};

//==============================================================================
//==============================================================================
//==============================================================================
class ENV;
class SmoothedParameter : public RuntimeListener
{
    friend class SmoothManager;

    SmoothManager*const smooth_manager;

    mono_AudioSampleBuffer<1> values;
    mono_AudioSampleBuffer<1> values_modulated;

    Parameter*const param_to_smooth;
    float const max_value;
    float const min_value;

    float last_value;
    float last_target;
    float difference_per_sample;
    int samples_left;
    bool buffer_is_linear_up_to_date_filled;

    class ModulatorSignalSmoother
    {
        float samples_left_max;
        int samples_left;

    public:
        void reset( float sample_rate_ ) noexcept;
        float attack( float current_modulator_signal_ ) noexcept;
        float release_amount() noexcept;
        bool is_released() const noexcept;

        COLD ModulatorSignalSmoother() noexcept;
        COLD ~ModulatorSignalSmoother() noexcept;
    } modulator_smoother;
    float last_modulator;
    bool was_modulated_last_time;

    float amp_switch_samples_left_max;
    int amp_switch_samples_left;
    float last_amp_automated;
    float last_amp_valued;
    bool was_automated_last_time;

    COLD void sample_rate_changed( double ) noexcept override;
    COLD void block_size_changed() noexcept override;

public:
    void smooth( int glide_motor_time_in_samples, int num_samples_ ) noexcept;
    void process_modulation( const bool is_modulated_, const float*modulator_buffer_, int num_samples_ ) noexcept;
    void process_amp( bool use_env_, ENV*env_, float*amp_buffer_, int num_samples_ ) noexcept;

    inline const float* get_smoothed_buffer() const noexcept
    {
        return values.getReadPointer();
    }
    inline const float* get_smoothed_modulated_buffer() const noexcept
    {
        return values_modulated.getReadPointer();
    }
    inline float operator[] ( int sid ) const noexcept
    {
        return values.getReadPointer()[sid];
    }
    void reset() noexcept {}

    COLD SmoothedParameter( SmoothManager*const smooth_manager_, Parameter*const param_to_smooth_ ) noexcept;
    COLD ~SmoothedParameter() noexcept;

    COLD void set_offline() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SmoothedParameter)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
struct LFOData
{
    Parameter speed;

    //==========================================================================
    COLD LFOData( SmoothManager*smooth_manager_, int id_ ) noexcept;
    COLD ~LFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOData)
};
static bool is_integer( float value_ ) noexcept
{
    return value_ == int(value_);
}

static float get_lfo_speed_multi( float speed_ ) noexcept
{
    float factor = 1;
    if( speed_ <= 6 )
    {
        if( speed_ <= 0 )
        {
            factor =  16.0f; //return "16/1";
        }
        else if( speed_ <= 1 )
        {
            factor = 12.0f + 4.0f*(1.0f-speed_); //return "12/1";
        }
        else if( speed_ <= 2 )
        {
            factor = 8.0f + 4.0f*(1.0f-(speed_-1));
        }
        else if( speed_ <= 3 )
        {
            factor = 4.0f + 4.0f*(1.0f-(speed_-2));
        }
        else if( speed_ <= 4 )
        {
            factor = 3 + (1.0f-(speed_-3));
        }
        else if( speed_ <= 5 )
        {
            factor = 2 + (1.0f-(speed_-4));
        }
        else if( speed_ <= 6 )
        {
            factor = 1 + (1.0f-(speed_-5));
        }
    }
    else if( speed_ < 17 )
    {
        factor = 0;
        if( speed_ <= 7 )
        {
            factor = 3.0f/4;
            factor += (1.0f-factor)*(1.0f-(speed_-6));
        }
        else if( speed_ <= 8 )
        {
            factor = 1.0f/2;
            factor += (3.0f/4-factor)*(1.0f-(speed_-7));
        }
        else if( speed_ <= 9 )
        {
            factor = 1.0f/3;
            factor += (1.0f/2-factor)*(1.0f-(speed_-8));
        }
        else if( speed_ <= 10 )
        {
            factor = 1.0f/4;
            factor += (1.0f/3-factor)*(1.0f-(speed_-9));
        }
        else if( speed_ <= 11 )
        {
            factor = 1.0f/8;
            factor += (1.0f/4-factor)*(1.0f-(speed_-10));
        }
        else if( speed_ <= 12 )
        {
            factor = 1.0f/12;
            factor += (1.0f/8-factor)*(1.0f-(speed_-11));
        }
        else if( speed_ <= 13 )
        {
            factor = 1.0f/16;
            factor += (1.0f/12-factor)*(1.0f-(speed_-12));
        }
        else if( speed_ <= 14 )
        {
            factor = 1.0f/24;
            factor += (1.0f/16-factor)*(1.0f-(speed_-13));
        }
        else if( speed_ <= 15 )
        {
            factor = 1.0f/32;
            factor += (1.0f/24-factor)*(1.0f-(speed_-14));
        }
        else if( speed_ <= 16 )
        {
            factor = 1.0f/64;
            factor += (1.0f/32-factor)*(1.0f-(speed_-15));
        }
        else
        {
            factor = 1.0f/128;
            factor += (1.0f/64-factor)*(1.0f-(speed_-15));
        }
    }

    return factor;
}
static float lfo_speed_in_hertz( float speed_, RuntimeInfo*info_, float sample_rate_ ) noexcept
{
    const float bars_per_sec = info_->bpm/4/60;
    const float cycles_per_sec = bars_per_sec/get_lfo_speed_multi( speed_ );
    return cycles_per_sec;
}
static String get_lfo_speed_multi_as_text( float speed_, RuntimeInfo*info_, float sample_rate_ ) noexcept
{
    if( speed_ <= 6 )
    {
        if( speed_ <= 0 )
        {
            return "16/1";
        }
        else if( speed_ == 1 )
        {
            return "12/1";
        }
        else if( speed_ == 2 )
        {
            return "8/1";
        }
        else if( speed_ == 3 )
        {
            return "4/1";
        }
        else if( speed_ == 4 )
        {
            return "3/1";
        }
        else if( speed_ == 5 )
        {
            return "2/1";
        }
        else if( speed_ == 6 )
        {
            return "1/1";
        }

        return String(round001(lfo_speed_in_hertz( speed_, info_, sample_rate_ )));
    }
    else if( speed_ <= 17 )
    {
        if( speed_ == 7 )
        {
            return "3/4";
        }
        else if( speed_ == 8 )
        {
            return "1/2";
        }
        else if( speed_ == 9 )
        {
            return "1/3";
        }
        else if( speed_ == 10 )
        {
            return "1/4";
        }
        else if( speed_ == 11 )
        {
            return "1/8";
        }
        else if( speed_ == 12 )
        {
            return "1/12";
        }
        else if( speed_ == 13 )
        {
            return "1/16";
        }
        else if( speed_ == 14 )
        {
            return "1/24";
        }
        else if( speed_ == 15 )
        {
            return "1/32";
        }
        else if( speed_ == 16 )
        {
            return "1/64";
        }
        else if( speed_ == 17 )
        {
            return "1/128";
        }

        return String(round001(lfo_speed_in_hertz( speed_, info_, sample_rate_ )));
    }
    else
    {
        return MidiMessage::getMidiNoteName(frequencyToMidi(midiToFrequency(33+speed_-18)),true,true,0);
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
struct MFOData
{
    Parameter speed;

    Parameter wave;
    SmoothedParameter wave_smoother;

    Parameter phase_shift;
    SmoothedParameter phase_shift_smoother;

    //==========================================================================
    COLD MFOData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~MFOData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MFOData)
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
struct FMOscData
{
    Parameter fm_freq;
    SmoothedParameter fm_freq_smoother;

    BoolParameter sync;
    Parameter fm_swing;
    SmoothedParameter fm_swing_smoother;
    Parameter fm_shape;
    SmoothedParameter fm_shape_smoother;

    ModulatedParameter master_shift;
    SmoothedParameter master_shift_smoother;

    //==========================================================================
    COLD FMOscData( SmoothManager*const smooth_manager_ ) noexcept;
    COLD ~FMOscData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMOscData)
};

//==============================================================================
//==============================================================================
//==============================================================================
struct OSCData
{
    const int id;

    BoolParameter sync;

    Parameter wave;
    SmoothedParameter wave_smoother;
    Parameter fm_amount;
    SmoothedParameter fm_amount_smoother;
    ModulatedParameter tune;
    SmoothedParameter tune_smoother;

    BoolParameter is_lfo_modulated;

    // FOR UI FEEDBACK
    float last_modulation_value;

    //==========================================================================
    COLD OSCData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~OSCData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCData)
};

//==============================================================================
//==============================================================================
//==============================================================================
#define MIN_ENV_TIMES 1 // 15
#define MAX_ENV_TIMES 5000-MIN_ENV_TIMES // 15
struct ENVData
{
    const int id;

    Parameter attack;
    IntParameter max_attack_time;
    Parameter decay;
    IntParameter max_decay_time;
    Parameter sustain;
    SmoothedParameter sustain_smoother;
    Parameter sustain_time;
    Parameter release;
    IntParameter max_release_time;

    Parameter shape;
    SmoothedParameter shape_smoother;

    //==========================================================================
    COLD ENVData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ENVData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ENVData)
};
static inline void copy( ENVData* dest_, const ENVData* src_, bool include_max_times_ ) noexcept
{
    dest_->attack = src_->attack;
    dest_->max_attack_time = src_->max_attack_time;
    dest_->decay = src_->decay;
    dest_->max_decay_time = src_->max_decay_time;
    dest_->sustain = src_->sustain;
    dest_->sustain_time = src_->sustain_time;
    dest_->release = src_->release;
    dest_->max_release_time = src_->max_release_time;

    dest_->shape = src_->shape;
}

//==============================================================================
//==============================================================================
//==============================================================================
struct FilterData
{
    IntParameter filter_type;
    Parameter adsr_lfo_mix;
    SmoothedParameter adsr_lfo_mix_smoother;

    ModulatedParameter distortion;
    SmoothedParameter distortion_smoother;
    BoolParameter modulate_distortion;

    ModulatedParameter cutoff;
    SmoothedParameter cutoff_smoother;
    BoolParameter modulate_cutoff;

    ModulatedParameter resonance;
    SmoothedParameter resonance_smoother;
    BoolParameter modulate_resonance;

    ModulatedParameter pan;
    SmoothedParameter pan_smoother;
    BoolParameter modulate_pan;
    ModulatedParameter output;
    SmoothedParameter output_smoother;
    BoolParameter modulate_output;

    ArrayOfParameters input_sustains;
    OwnedArray<SmoothedParameter> input_smoothers;
    OwnedArray<ENVData> input_envs;
    ArrayOfBoolParameters input_holds;

    ENVData*const env_data;

public:
    //==========================================================================
    COLD FilterData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~FilterData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( FilterData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ArpSequencerData
{
    BoolParameter is_on;

    ArrayOfBoolParameters step;
    ArrayOfIntParameters tune;
    ArrayOfParameters velocity;
    OwnedArray<SmoothedParameter> velocity_smoothers;

    IntParameter shuffle;
    BoolParameter connect;
    IntParameter speed_multi;

    //==========================================================================
    COLD ArpSequencerData( int id_ ) noexcept;
    COLD ~ArpSequencerData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ArpSequencerData )

    //==========================================================================
    static StringRef speed_multi_to_text( int speed_multi_ ) noexcept;
    static double speed_multi_to_value( int speed_multi_ ) noexcept;

    static StringRef shuffle_to_text( int speed_multi_ ) noexcept;
    static float shuffle_to_value( int speed_multi_ ) noexcept;
};

//==========================================================================
inline double ArpSequencerData::speed_multi_to_value( int speed_multi_ ) noexcept
{
    switch( speed_multi_ )
    {
    case 0 :
        return 1;
    case 1 :
        return 2;
    case -1 :
        return 0.5;
    case 2 :
        return 3;
    case -2 :
        return (1.0/3);
    case 3 :
        return 4;
    case -3 :
        return (1.0/4);
    case 4 :
        return 5;
    case -4 :
        return (1.0/5);
    case 5 :
        return 6;
    case -5 :
        return (1.0/6);
    case 6 :
        return 7;
    case -6 :
        return (1.0/7);
    case 7 :
        return 8;
    case -7 :
        return (1.0/8);
    case 8 :
        return 9;
    case -8 :
        return (1.0/9);
    case 9 :
        return 10;
    case -9 :
        return (1.0/10);
    case 10 :
        return 11;
    case -10 :
        return (1.0/11);
    case 11 :
        return 12;
    case -11 :
        return (1.0/12);
    case 12 :
        return 13;
    case -12 :
        return (1.0/13);
    case 13 :
        return 14;
    case -13 :
        return (1.0/14);
    case 14 :
        return 15;
    case -14 :
        return (1.0/15);
    case 15 :
        return 16;
    default : // case -15 :
        return (1.0/16);
    }
}

//==============================================================================
inline StringRef ArpSequencerData::speed_multi_to_text( int speed_multi_ ) noexcept
{
    switch( speed_multi_ )
    {
    case 0 :
        return "x1";
    case 1 :
        return "x2";
    case -1 :
        return "/2";
    case 2 :
        return "x3";
    case -2 :
        return "/3";
    case 3 :
        return "x4";
    case -3 :
        return "/4";
    case 4 :
        return "x5";
    case -4 :
        return "/5";
    case 5 :
        return "x6";
    case -5 :
        return "/6";
    case 6 :
        return "x7";
    case -6 :
        return "/7";
    case 7 :
        return "x8";
    case -7 :
        return "/8";
    case 8 :
        return "x9";
    case -8 :
        return "/9";
    case 9 :
        return "x10";
    case -9 :
        return "/10";
    case 10 :
        return "x11";
    case -10 :
        return "/11";
    case 11 :
        return "x12";
    case -11 :
        return "/12";
    case 12 :
        return "x16";
    case -12 :
        return "/13";
    case 13 :
        return "x14";
    case -13 :
        return "/14";
    case 14 :
        return "x15";
    case -14 :
        return "/15";
    case 15 :
        return "x16";
    default : // -15 :
        return "/16";
    }
}

//==============================================================================
inline float ArpSequencerData::shuffle_to_value( int suffle_ ) noexcept
{
    switch( suffle_ )
    {
    case 0 :
        return 0;
    case 1 :
        return 1.0f/128;
    case 2 :
        return 1.0f/96;
    case 3 :
        return 1.0f/64;
    case 4 :
        return 1.0f/48;
    case 5 :
        return 1.0f/32;
    case 6 :
        return 1.0f/24;
    case 7 :
        return 1.0f/16;
    case 8 :
        return 1.0f/12;
    case 9 :
        return 1.0f/8;
    case 10 :
        return 2.0f/8;
    case 11 :
        return 3.0f/8;
    case 12 :
        return 4.0f/8;
    case 13 :
        return 5.0f/8;
    case 14 :
        return 6.0f/8;
    case 15 :
        return 7.0f/8;
    default :
        return 1;
    }
}

//==============================================================================
inline StringRef ArpSequencerData::shuffle_to_text( int suffle_ ) noexcept
{
    switch( suffle_ )
    {
    case 0 :
        return "OFF";
    case 1 :
        return "1/128";
    case 2 :
        return "1/96";
    case 3 :
        return "1/64";
    case 4 :
        return "1/32";
    case 5 :
        return "1/32";
    case 6 :
        return "1/24";
    case 7 :
        return "1/16";
    case 8 :
        return "1/12";
    case 9 :
        return "1/8";
    case 10 :
        return "2/8";
    case 11 :
        return "3/8";
    case 12 :
        return "4/8";
    case 13 :
        return "5/8";
    case 14 :
        return "6/8";
    case 15 :
        return "7/8";
    case 16 :
        return "1/1";
    }
}

//==============================================================================
//==============================================================================
//==============================================================================
static inline int get_low_pass_band_frequency( int band_id_ ) noexcept
{
    switch(band_id_)
    {
    case 0 :
        return 80;
    case 1 :
        return 160;
    case 2 :
        return 320;
    case 3 :
        return 640;
    case 4 :
        return 1280;
    case 5 :
        return 2660;
    default :
        return 22000;
    }
}
static inline int get_high_pass_band_frequency( int band_id_ ) noexcept
{
    switch(band_id_)
    {
    case 0 :
        return 15;
    case 1 :
        return 80;
    case 2 :
        return 160;
    case 3 :
        return 320;
    case 4 :
        return 640;
    case 5 :
        return 1280;
    default :
        return 2660;
    }
}

//==============================================================================
struct EQData
{
    ArrayOfParameters velocity;
    OwnedArray<SmoothedParameter> velocity_smoothers;
    ArrayOfBoolParameters hold;

    Parameter bypass;
    SmoothedParameter bypass_smoother;

    OwnedArray<ENVData> envs;

public:
    //==========================================================================
    COLD EQData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~EQData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( EQData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ReverbData
{
    Parameter room;
    SmoothedParameter room_smoother;
    Parameter dry_wet_mix;
    SmoothedParameter dry_wet_mix_smoother;
    Parameter width;
    SmoothedParameter width_smoother;

    Parameter pan;
    SmoothedParameter pan_smoother;

    //==========================================================================
    COLD ReverbData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ReverbData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ReverbData )
};

//==============================================================================
//==============================================================================
//==============================================================================
struct ChorusData
{
    Parameter modulation;
    SmoothedParameter modulation_smoother;

    Parameter pan;
    SmoothedParameter pan_smoother;

public:
    //==========================================================================
    COLD ChorusData( SmoothManager*const smooth_manager_, int id_ ) noexcept;
    COLD ~ChorusData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( ChorusData )
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class MorphGroup : public Timer, ParameterListener
{
    MorphGroup* left_morph_source;
    MorphGroup* right_morph_source;

    friend class MoniqueSynthData;
    Array< Parameter* > params;
    float last_power_of_right;
    Array< BoolParameter* > switch_bool_params;
    bool current_switch;
    Array< IntParameter* > switch_int_params;

public:
    //==========================================================================
    inline void morph( float morph_amount_ ) noexcept;
    inline void morph_switchs( bool left_right_ ) noexcept;

    // return true if the morph was successful
    inline bool morph( const Parameter* original_param_,  float* value_target_buffer_, float* mod_target_buffer_, const float* morph_amount_, int num_samples_ ) noexcept;

private:
    //==========================================================================
    Array< float > sync_param_deltas;
    Array< float > sync_modulation_deltas;
    void run_sync_morph() noexcept;
    int current_callbacks;
    void timerCallback() override;

private:
    //==========================================================================
    // WILL ONLY BE CALLED IN THE MASTER MORPH GROUP, COZ THE SUB GROUBS DOES NOT LISTEN THE PARAMS
    // UPDATES THE LEFT AND RIGHT SOURCES
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_modulation_value_changed( Parameter* param_ ) noexcept override;

public:
    //==========================================================================
    // INIT
    COLD MorphGroup() noexcept;
    COLD ~MorphGroup() noexcept;

    COLD void register_parameter( Parameter* param_, bool is_master_ ) noexcept;
    COLD void register_switch_parameter( BoolParameter* param_, bool is_master_ ) noexcept;
    COLD void register_switch_parameter( IntParameter* param_, bool is_master_ ) noexcept;

    COLD void set_sources( MorphGroup* left_source_, MorphGroup* right_source_,
                           float current_morph_amount_, bool current_switch_state_ ) noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphGroup)
};


//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
#ifdef IS_STANDALONE
#define THREAD_LIMIT 4
#else
#define THREAD_LIMIT 0
#endif

class MoniqueSynthesiserVoice;
struct MoniqueSynthData : ParameterListener
{
    UiLookAndFeel*const ui_look_and_feel; // WILL BE NULL FOR MORPH DATA
    MoniqueAudioProcessor*const audio_processor; // WILL BE NULL FOR MORPH DATA

    SmoothManager*const smooth_manager; // TODO is nowhere deleted
    RuntimeNotifyer*const runtime_notifyer;
    RuntimeInfo*const runtime_info;
    DataBuffer*const data_buffer;
    MoniqueSynthesiserVoice*voice; // WILL BE SET BY THE PROCESSOR

    //==============================================================================
    const float*const sine_lookup;
    const float*const cos_lookup;
    const float*const exp_lookup;

    const int id;

    Parameter volume;
    SmoothedParameter volume_smoother;
    Parameter glide;
    SmoothedParameter glide_smoother;
    Parameter delay;
    SmoothedParameter delay_smoother;
    Parameter delay_pan;
    SmoothedParameter delay_pan_smoother;
    Parameter effect_bypass;
    SmoothedParameter effect_bypass_smoother;
    Parameter shape;
    SmoothedParameter shape_smoother;
    Parameter distortion;
    SmoothedParameter distortion_smoother;
    IntParameter octave_offset;
    IntParameter note_offset;

    BoolParameter sync;
    Parameter speed;

    IntParameter glide_motor_time;
    IntParameter velocity_glide_time;

    BoolParameter ctrl;

    Parameter midi_pickup_offset;

    // OSCILLOSCOPE SETTINGS
    BoolParameter osci_show_osc_1;
    BoolParameter osci_show_osc_2;
    BoolParameter osci_show_osc_3;
    BoolParameter osci_show_flt_env_1;
    BoolParameter osci_show_flt_env_2;
    BoolParameter osci_show_flt_env_3;
    BoolParameter osci_show_flt_1;
    BoolParameter osci_show_flt_2;
    BoolParameter osci_show_flt_3;
    BoolParameter osci_show_eq;
    BoolParameter osci_show_out;
    BoolParameter osci_show_out_env;
    Parameter osci_show_range;

    BoolParameter auto_close_env_popup;
    BoolParameter auto_switch_env_popup;

    BoolParameter is_osci_open;

    BoolParameter keep_arp_always_on;
    BoolParameter keep_arp_always_off;

    // MULTITHREADING
    IntParameter num_extra_threads;

    // SETTINGS
    BoolParameter animate_envs;
    BoolParameter show_tooltips;
    BoolParameter bind_sustain_and_sostenuto_pedal;
    BoolParameter sliders_in_rotary_mode;
    IntParameter sliders_sensitivity;
    Parameter ui_scale_factor;

    ScopedPointer< ENVData > env_data;

    OwnedArray< LFOData > lfo_datas;
    OwnedArray< MFOData > mfo_datas;
    OwnedArray< OSCData > osc_datas;
    ScopedPointer<FMOscData> fm_osc_data;
    OwnedArray< FilterData > filter_datas;
    ScopedPointer< EQData > eq_data;
    ScopedPointer< ArpSequencerData > arp_sequencer_data;
    ScopedPointer< ReverbData > reverb_data;
    ScopedPointer< ChorusData > chorus_data;

private:
    // ==============================================================================
    Array< Parameter* > saveable_parameters;
    Array< float > saveable_backups;
    Array< Parameter* > global_parameters;
    Array< Parameter* > all_parameters;
    COLD void colect_saveable_parameters() noexcept;
    COLD void colect_global_parameters() noexcept;

public:
    // TODO
    inline Array< Parameter* >& get_atomateable_parameters() noexcept
    {
        return saveable_parameters;
    }
    inline Array< Parameter* >& get_global_parameters() noexcept
    {
        return global_parameters;
    }
    inline Array< Parameter* >& get_all_parameters() noexcept
    {
        return all_parameters;
    }

    // ==============================================================================
private:
    friend class MoniqueAudioProcessor;
    friend class ContainerDeletePolicy< MoniqueSynthData >;
    COLD MoniqueSynthData( DATA_TYPES data_type,
                           UiLookAndFeel*look_and_feel_,
                           MoniqueAudioProcessor*const audio_processor_,
                           RuntimeNotifyer*const runtime_notifyer_,
                           RuntimeInfo*const info_,
                           DataBuffer*data_buffer_,
                           SmoothManager*smooth_manager = nullptr /* NOTE: the master data owns the manager, but the morph groups will be smoothed*/
                         ) noexcept;
    COLD ~MoniqueSynthData() noexcept;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR( MoniqueSynthData )

public:
    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // MORPH
    ArrayOfParameters morhp_states;
    ArrayOfBoolParameters is_morph_modulated;
    ArrayOfParameters morhp_automation_power;
    ArrayOfBoolParameters morhp_switch_states;
    Parameter linear_morhp_state;
    IntParameter morph_motor_time;

public:
    ScopedPointer<MorphGroup> morph_group_1, morph_group_2, morph_group_3, morph_group_4;
private:
    OwnedArray< MoniqueSynthData > left_morph_sources;
    OwnedArray< MoniqueSynthData > right_morph_sources;
    StringArray left_morph_source_names;
    StringArray right_morph_source_names;
public:
    const String& get_morph_source_name( int id_abs_ ) const noexcept;
private:

    COLD void init_morph_groups( DATA_TYPES data_type ) noexcept;

    CriticalSection morph_lock;

public:
    inline float get_morph_state( int morpher_id_ ) const noexcept;
    inline bool get_morph_switch_state( int morpher_id_ ) const noexcept;
    inline void morph( int morpher_id_, float morph_amount_left_to_right_, bool force_ = false ) noexcept;
    inline void morph_switch_buttons( int morpher_id_, bool do_switch_ = true ) noexcept;
    inline void run_sync_morph() noexcept;

private:
    void parameter_value_changed( Parameter* param_ ) noexcept override;
    void parameter_value_changed_by_automation( Parameter* param_ ) noexcept override;

public:
    // COPY THE CURRENT STATE TO THE SOURCES
    void set_morph_source_data_from_current( int morpher_id_, bool left_or_right_, bool run_sync_morph_ ) noexcept;
    void refresh_morph_programms() noexcept;
    bool try_to_load_programm_to_left_side( int morpher_id_, int bank_id_, int index_ ) noexcept;
    bool try_to_load_programm_to_right_side( int morpher_id_, int bank_id_, int index_ ) noexcept;

private:
    // ==============================================================================
    // ==============================================================================
    // ==============================================================================
    // FILE IO
    StringArray banks;
    Array< StringArray > program_names_per_bank;
    String last_program;
    String last_bank;

    int current_program;
    int current_program_abs;
    int current_bank;

public:
    // ==============================================================================
    static void refresh_banks_and_programms( MoniqueSynthData& synth_data ) noexcept;
private:
    void calc_current_program_abs() noexcept;

    static void update_banks( StringArray& ) noexcept;
    static void update_bank_programms( MoniqueSynthData& synth_data, int bank_id_, StringArray& program_names_ ) noexcept;

public:
    // ==============================================================================
    const StringArray& get_banks() noexcept;
    const StringArray& get_programms( int bank_id_ ) noexcept;

    // ==============================================================================
    void set_current_bank( int bank_index_ ) noexcept;
    void set_current_program( int programm_index_ ) noexcept;
    void set_current_program_abs( int programm_index_ ) noexcept;

    int get_current_bank() const noexcept;
    int get_current_program() const noexcept;
    const StringArray& get_current_bank_programms() const noexcept;
    String alternative_program_name;

    const String error_string;
    int get_current_programm_id_abs() const noexcept;
    const String& get_current_program_name_abs() const noexcept;
    const String& get_program_name_abs(int id_) const noexcept;

    // ==============================================================================
    void create_internal_backup( const String& programm_name_, const String& bank_name_  ) noexcept;
    bool create_new( const String& new_name_ ) noexcept;
    bool rename( const String& new_name_ ) noexcept;
    bool replace() noexcept;
    bool remove() noexcept;

    bool load( bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;
    bool load_prev() noexcept;
    bool load_next() noexcept;
private:
    bool load( const String bank_name_, const String program_name_, bool load_morph_groups = true, bool ignore_warnings_ = false ) noexcept;

public:
    // ==============================================================================
    void load_default() noexcept;
    void save_to( XmlElement* xml ) noexcept;
    void read_from( const XmlElement* xml ) noexcept;

private:
    bool write2file( const String& bank_name_, const String& program_name_ ) noexcept;

public:
    void save_settings() const noexcept;
    void ask_and_save_if_changed( bool with_new_option = false ) noexcept;
    void load_settings() noexcept;

public:
    // ==============================================================================
    void save_midi() const noexcept;
    void read_midi() noexcept;

public:
    // ==============================================================================
    void get_full_adstr( ENVData&env_data_,Array< float >& curve ) noexcept;
    void get_full_mfo( MFOData&mfo_data_,Array< float >& curve ) noexcept;
};

//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
class SHARED
{
public:
    int num_instances ;
    ENVData* env_clipboard;
    juce_DeclareSingleton( SHARED, false );

    SHARED() : num_instances(0), env_clipboard(nullptr) {}
};

#endif
