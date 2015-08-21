/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_C21DF0D02B821D7E__
#define __JUCE_HEADER_C21DF0D02B821D7E__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

#define MAX_BUFFER_SIZE (44100*20)
template<class type, int max_size = MAX_BUFFER_SIZE>
class EndlessBuffer {
    int position;
    int last_switch_point;
    int past_offset;

    int size;
    Array<type, DummyCriticalSection, max_size> data;
    Array<bool, DummyCriticalSection, max_size> switch_positions;

public:
    void add( type value_, bool is_switch_ = true ) {
        position++;
        if( position >= (MAX_BUFFER_SIZE-1) )
            position = 0;

        data.getReference(position) = value_;

        if( is_switch_ )
        {
            last_switch_point = position;
            switch_positions.getReference(position) = true;
        }
        else
        {
            switch_positions.getReference(position) = false;
        }
    }

    int get_current_position( int to_past_ ) {
        int count_past = position - size - to_past_;
        for( past_offset = size; ; ++past_offset, --count_past )
        {
            if( count_past < 0 )
                count_past = MAX_BUFFER_SIZE-1;
            if( count_past == position+1 ) {
                count_past = position - size;
                break;
            }

            if( switch_positions[count_past] ) {
                break;
            }
        }
        if( count_past < 0 )
            count_past = MAX_BUFFER_SIZE-1;

        return count_past;
    }

    int get_past_offset() {
        int value = past_offset-size;
        if( value <0 )
            value = 0;
        return value;
    }

    type get( int start_position_, int i_ ) {
        int pos = start_position_+i_;
        if( pos > (MAX_BUFFER_SIZE-1) ) {
            pos = pos - (MAX_BUFFER_SIZE-1);
        }

        return data[pos];
    }

    void set_size( int size_ ) {
        size = size_;
    }

    EndlessBuffer() : position(0), last_switch_point(0), past_offset(0), size(1024) {
        data.resize( MAX_BUFFER_SIZE );
        switch_positions.resize( MAX_BUFFER_SIZE );
    }
};
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class mono_AmpPainter  : public Component,
    public mono_UiRefreshable,
    public SliderListener,
    public ButtonListener
{
public:
    //==============================================================================
    mono_AmpPainter ();
    ~mono_AmpPainter();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    const float original_w;
    const float original_h;

private:
    int resizer;

    OwnedArray<EndlessBuffer<float>> filter_values;
    OwnedArray<EndlessBuffer<float>> filter_env_values;
    EndlessBuffer<float> eq_values;
    EndlessBuffer<float> values_env;
    EndlessBuffer<float> values;
    OwnedArray<EndlessBuffer<float>> osc_values;
    
    Array<EndlessBuffer<float>*> buffers;

    Array<bool> show_filter;
    Array<bool> show_filter_env;
    bool show_eq;
    bool show_out_env;
    bool show_out;
    Array<bool> show_osc;

public:
    void add_filter( int id_, const float* values_, int num_samples_ ) noexcept;
    void add_filter_env( int id_, const float* values_, int num_samples_ ) noexcept;
    void add_eq( const float* values_, int num_samples_ ) noexcept;
    void add_out_env( const float* values_, int num_samples_ ) noexcept;
    void add_out( const float* values_, int num_samples_ ) noexcept;
    void add_osc( int id_, const float* values_, const float* is_switch_values, int num_samples_ ) noexcept;

private:
    void refresh() noexcept override;

    void refresh_buttons();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider* sliderThatWasMoved);
    void buttonClicked (Button* buttonThatWasClicked);

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Slider> sl_osc_octave_3;
    ScopedPointer<TextButton> osc_1;
    ScopedPointer<TextButton> osc_2;
    ScopedPointer<TextButton> osc_3;
    ScopedPointer<TextButton> lfo_1;
    ScopedPointer<TextButton> out;
    ScopedPointer<TextButton> f_1;
    ScopedPointer<TextButton> f_2;
    ScopedPointer<TextButton> f_3;
    ScopedPointer<TextButton> f_env_1;
    ScopedPointer<TextButton> f_env_2;
    ScopedPointer<TextButton> f_env_3;
    ScopedPointer<TextButton> out_env;
    ScopedPointer<Component> drawing_area;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (mono_AmpPainter)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_C21DF0D02B821D7E__