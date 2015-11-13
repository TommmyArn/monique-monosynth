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

#ifndef __JUCE_HEADER_5A0D36FB903763FA__
#define __JUCE_HEADER_5A0D36FB903763FA__

//[Headers]     -- You can add your own extra header files here --
#include "App_h_includer.h"

class AppInstanceStore;

class Monique_Ui_Refresher;

class Monique_Ui_MainwindowPopup;
class Monique_Ui_MidiIO;
class Monique_Ui_MorphConfig;
class Monique_Ui_GlobalSettings;
class Monique_Ui_ENVPopup;
class Monique_Ui_MFOPopup;
class Monique_Ui_OptionPopup;
class Monique_Ui_AmpPainter;

class MoniqueSynthesiserVoice;
class MoniqueSynthData;

class Monique_Ui_SegmentedMeter;
class MIDIControl;
class Monique_Ui_DualSlider;

class ModulationSliderConfigBase;

class ENVData;
class LFOData;
enum COLOUR_THEMES;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Monique_Ui_Mainwindow  : public AudioProcessorEditor,
                               public Monique_Ui_Refreshable,
                               public AsyncUpdater,
                               public ButtonListener,
                               public ComboBoxListener
{
public:
    //==============================================================================
    Monique_Ui_Mainwindow (Monique_Ui_Refresher*ui_refresher_);
    ~Monique_Ui_Mainwindow();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int last_refreshed_note;

    MoniqueAudioProcessor* audio_processor;
    MoniqueSynthesiserVoice* voice;

    ScopedPointer<Monique_Ui_MainwindowPopup> popup;
    bool last_ctrl_mode;
    bool is_in_help_mode;
    ScopedPointer<Monique_Ui_MidiIO> editor_midiio;
    ScopedPointer<Monique_Ui_MorphConfig> editor_morph;
    ScopedPointer<Monique_Ui_GlobalSettings> editor_global_settings;
    ScopedPointer<Monique_Ui_ENVPopup> env_popup;
    ScopedPointer<Monique_Ui_MFOPopup> mfo_popup;
    ScopedPointer<Monique_Ui_OptionPopup> option_popup;
    Monique_Ui_AmpPainter*amp_painter;

    void refresh() noexcept override;
    void update_tooltip_handling( bool is_help_key_down_ ) noexcept;
    void show_current_voice_data();
    void handleAsyncUpdate() override;
    void show_programs_and_select(bool force);
    void update_slider_return_values() noexcept;
    void restore_slider_value( Component*slider_, int type_ /* 0 = user, 1 = factory, 2, program */ ) noexcept;
    void global_slider_settings_changed( Component*parent_ ) noexcept;
    void show_ctrl_state();
    Parameter* find_parameter_in_dual_sliders( const Component* comp_ ) noexcept;
    Parameter* find_back_parameter_in_dual_sliders( const Component* comp_ ) noexcept;
    void show_info_popup( Component* comp_, MIDIControl* midi_conrtrol_ , bool force_turn_on_ = false );
    void close_all_subeditors();
    void open_env_popup( ENVData*const env_data_, Parameter*const sustain_, Button*const for_comp_, Monique_Ui_DualSlider*slider_, bool has_negative_sustain_ ) noexcept;
    void open_env_popup( Monique_Ui_DualSlider*dual_slider_ ) noexcept;
    void open_mfo_popup( LFOData*const mfo_data_, Button*const for_comp_, Monique_Ui_DualSlider*slider_, COLOUR_THEMES theme_ ) noexcept;
    void open_mfo_popup( Monique_Ui_DualSlider*dual_slider_ ) noexcept;
    void open_option_popup( Component*const for_comp_,
                            BoolParameter*param_a_, BoolParameter*param_b_,
                            StringRef text_a_, StringRef text_b_,
                            StringRef tool_tip_a_, StringRef tool_tip_b_ ) noexcept;
    bool handle_keep_env_open( ModulationSliderConfigBase*const caller_config_ ) noexcept;
    bool handle_keep_mfo_open( ModulationSliderConfigBase*const caller_config_ ) noexcept;
    void resize_subeditors();
    void open_midi_editor_if_closed() noexcept;
    void open_setup_editor_if_closed() noexcept;
    int flash_counter;
    void flash_midi_editor_button() noexcept;

    float last_shuffle;
    Array< TextButton* > sequence_buttons;
    void resize_sequence_buttons();
    void switch_finalizer_tab( bool fx_ );
    Array< Monique_Ui_DualSlider* > dual_sliders;
    void update_size();

    Array<int> last_morpher_index;

    const float original_w;
    const float original_h;
    int last_bank;
    int last_programm;
    enum EDIT_TYPES
    {
        LOAD,
        REPLACE,
        CREATE,
        RENAME,
        REMOVE,
        NOT_SET
    };
    EDIT_TYPES program_edit_type;

    bool is_ctrl_down;

    void mouseEnter (const MouseEvent& event) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    bool keyPressed (const KeyPress& key);
    bool keyStateChanged (const bool isKeyDown);
    void modifierKeysChanged (const ModifierKeys& modifiers);

    // Binary resources:
    static const char* empty_png;
    static const int empty_pngSize;
    static const char* _01hintergrundalles_svg;
    static const int _01hintergrundalles_svgSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    ScopedPointer< TooltipWindow > tooltipWindow;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> button_edit_input_env_1_3;
    ScopedPointer<TextButton> button_edit_input_env_1_2;
    ScopedPointer<TextButton> button_edit_input_env_1_1;
    ScopedPointer<TextButton> button_edit_input_env_2_1;
    ScopedPointer<TextButton> button_edit_input_env_2_2;
    ScopedPointer<TextButton> button_edit_input_env_2_3;
    ScopedPointer<TextButton> button_edit_input_env_3_3;
    ScopedPointer<TextButton> button_edit_input_env_3_2;
    ScopedPointer<TextButton> button_edit_input_env_3_1;
    ScopedPointer<TextButton> button_edit_mfo_1;
    ScopedPointer<TextButton> button_edit_mfo_2;
    ScopedPointer<TextButton> button_edit_mfo_3;
    ScopedPointer<TextButton> button_edit_mfo_4;
    ScopedPointer<TextButton> button_edit_lfo_3;
    ScopedPointer<TextButton> button_edit_lfo_2;
    ScopedPointer<TextButton> button_edit_lfo_1;
    ScopedPointer<TextButton> button_edit_input_env_band_7;
    ScopedPointer<Monique_Ui_DualSlider> eq_7;
    ScopedPointer<Monique_Ui_DualSlider> reverb_dry;
    ScopedPointer<TextButton> button_edit_input_env_band_1;
    ScopedPointer<Monique_Ui_DualSlider> eq_1;
    ScopedPointer<Monique_Ui_DualSlider> distortion;
    ScopedPointer<TextButton> button_edit_input_env_band_2;
    ScopedPointer<TextButton> button_edit_input_env_band_3;
    ScopedPointer<TextButton> button_edit_input_env_band_4;
    ScopedPointer<TextButton> button_edit_input_env_band_5;
    ScopedPointer<TextButton> button_edit_input_env_band_6;
    ScopedPointer<Monique_Ui_DualSlider> eq_6;
    ScopedPointer<Monique_Ui_DualSlider> eq_5;
    ScopedPointer<Monique_Ui_DualSlider> eq_4;
    ScopedPointer<Monique_Ui_DualSlider> eq_3;
    ScopedPointer<Monique_Ui_DualSlider> eq_2;
    ScopedPointer<TextButton> filter_type_bg_button_3;
    ScopedPointer<TextButton> filter_type_2_3;
    ScopedPointer<TextButton> filter_type_bg_button_2;
    ScopedPointer<TextButton> filter_type_2_2;
    ScopedPointer<TextButton> filter_type_bg_button_1;
    ScopedPointer<TextButton> filter_type_2_1;
    ScopedPointer<Label> label_lfo_1;
    ScopedPointer<Label> label_lfo_2;
    ScopedPointer<Label> label_lfo_3;
    ScopedPointer<Monique_Ui_DualSlider> morpher_1;
    ScopedPointer<Monique_Ui_DualSlider> morpher_2;
    ScopedPointer<Monique_Ui_DualSlider> morpher_4;
    ScopedPointer<Monique_Ui_DualSlider> morpher_3;
    ScopedPointer<Monique_Ui_DualSlider> lfo_1;
    ScopedPointer<Monique_Ui_DualSlider> lfo_2;
    ScopedPointer<Monique_Ui_DualSlider> lfo_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_11;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_12;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_13;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_8;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_7;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_6;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_input_2;
    ScopedPointer<TextButton> button_toggle_morph_buttons_1;
    ScopedPointer<TextButton> button_toggle_morph_buttons_2;
    ScopedPointer<TextButton> button_toggle_morph_buttons_3;
    ScopedPointer<TextButton> button_toggle_morph_buttons_4;
    ScopedPointer<Label> label_band_hz_5;
    ScopedPointer<Label> label_band_hz_6;
    ScopedPointer<Label> label_band_hz_4;
    ScopedPointer<Label> label_band_hz_1;
    ScopedPointer<Label> label_morph;
    ScopedPointer<Label> label_band_hz_7;
    ScopedPointer<Label> label_arpeggiator;
    ScopedPointer<Label> label_monique;
    ScopedPointer<TextButton> button_programm_replace;
    ScopedPointer<TextButton> button_programm_new;
    ScopedPointer<Monique_Ui_DualSlider> reverb_room;
    ScopedPointer<Monique_Ui_DualSlider> delay4;
    ScopedPointer<Monique_Ui_DualSlider> bypass;
    ScopedPointer<Monique_Ui_DualSlider> colour;
    ScopedPointer<Monique_Ui_DualSlider> delay2;
    ScopedPointer<Monique_Ui_DualSlider> chorus_modulation;
    ScopedPointer<Label> label_band_hz_2;
    ScopedPointer<Label> label_band_hz_3;
    ScopedPointer<Monique_Ui_DualSlider> speed_multi;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_3;
    ScopedPointer<MidiKeyboardComponent> keyboard;
    ScopedPointer<Monique_Ui_DualSlider> glide2;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_16;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_15;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_14;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_13;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_12;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_11;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_10;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_9;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_8;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_7;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_6;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_5;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_4;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_3;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_2;
    ScopedPointer<Monique_Ui_DualSlider> arp_step_1;
    ScopedPointer<Monique_Ui_DualSlider> shuffle;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_4;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_time_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_sustain_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_decay_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_1;
    ScopedPointer<TextButton> filter_type_3_1;
    ScopedPointer<TextButton> filter_type_3_2;
    ScopedPointer<TextButton> filter_type_3_3;
    ScopedPointer<TextButton> filter_type_5_1;
    ScopedPointer<TextButton> filter_type_5_2;
    ScopedPointer<TextButton> filter_type_5_3;
    ScopedPointer<TextButton> button_sequence_2;
    ScopedPointer<TextButton> button_sequence_3;
    ScopedPointer<TextButton> button_sequence_4;
    ScopedPointer<TextButton> button_sequence_5;
    ScopedPointer<TextButton> button_sequence_6;
    ScopedPointer<TextButton> button_sequence_7;
    ScopedPointer<TextButton> button_sequence_8;
    ScopedPointer<TextButton> button_sequence_9;
    ScopedPointer<TextButton> button_sequence_10;
    ScopedPointer<TextButton> button_sequence_11;
    ScopedPointer<TextButton> button_sequence_12;
    ScopedPointer<TextButton> button_sequence_13;
    ScopedPointer<TextButton> button_sequence_14;
    ScopedPointer<TextButton> button_sequence_15;
    ScopedPointer<TextButton> button_sequence_16;
    ScopedPointer<ComboBox> combo_programm;
    ScopedPointer<TextButton> button_programm_left;
    ScopedPointer<TextButton> button_programm_right;
    ScopedPointer<TextButton> button_open_oszi;
    ScopedPointer<TextButton> button_open_midi_io_settings;
    ScopedPointer<ComboBox> combo_bank;
    ScopedPointer<TextButton> button_programm_load;
    ScopedPointer<Monique_Ui_DualSlider> osc_1;
    ScopedPointer<Monique_Ui_DualSlider> osc_2;
    ScopedPointer<Monique_Ui_DualSlider> osc_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_cutoff_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_resonance_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_volume_3;
    ScopedPointer<Monique_Ui_DualSlider> adsr_lfo_mix;
    ScopedPointer<Monique_Ui_DualSlider> lfo_opt_2;
    ScopedPointer<Monique_Ui_DualSlider> lfo_opt_3;
    ScopedPointer<TextButton> button_sequence_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_release_4;
    ScopedPointer<Monique_Ui_DualSlider> volume;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_distortion_3;
    ScopedPointer<TextButton> button_arp_speed_XNORM;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_5;
    ScopedPointer<Monique_Ui_DualSlider> flt_attack_6;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_1;
    ScopedPointer<Monique_Ui_DualSlider> osc_wave_2;
    ScopedPointer<TextButton> button_programm_delete;
    ScopedPointer<TextButton> filter_type_6_1;
    ScopedPointer<TextButton> filter_type_6_2;
    ScopedPointer<TextButton> filter_type_6_3;
    ScopedPointer<TextButton> button_ctrl_toggle;
    ScopedPointer<Monique_Ui_DualSlider> speed;
    ScopedPointer<TextButton> button_open_morph;
    ScopedPointer<TextButton> effect_finalizer_switch;
    ScopedPointer<TextButton> button_values_toggle;
    ScopedPointer<Monique_Ui_DualSlider> octave_offset;
    ScopedPointer<Label> label_filter_inputs;
    ScopedPointer<Label> label_oscillators;
    ScopedPointer<Label> label_filter_envelope;
    ScopedPointer<Label> label_lfo;
    ScopedPointer<Label> label_filter_config;
    ScopedPointer<Label> label_filter_fx;
    ScopedPointer<Label> label_out;
    ScopedPointer<Label> label_amp_envelope;
    ScopedPointer<Label> label_glide;
    ScopedPointer<Label> label_speed;
    ScopedPointer<Label> label_tune;
    ScopedPointer<Label> label_fm;
    ScopedPointer<Monique_Ui_SegmentedMeter> volume_master_meter;
    ScopedPointer<TextButton> button_open_config2;
    ScopedPointer<Label> label_mod_mix;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_3;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_pan_1;
    ScopedPointer<TextButton> button_reset_arp_tune;
    ScopedPointer<TextButton> button_show_active_input_r_2_3;
    ScopedPointer<TextButton> button_show_active_input_l_2_3;
    ScopedPointer<TextButton> button_show_active_input_r_2_2;
    ScopedPointer<TextButton> button_show_active_input_l_2_2;
    ScopedPointer<TextButton> button_show_active_input_r_2_1;
    ScopedPointer<TextButton> button_show_active_input_l_2_1;
    ScopedPointer<TextButton> button_show_active_input_r_3_3;
    ScopedPointer<TextButton> button_show_active_input_l_3_3;
    ScopedPointer<TextButton> button_show_active_input_r_3_2;
    ScopedPointer<TextButton> button_show_active_input_l_3_2;
    ScopedPointer<TextButton> button_show_active_input_r_3_1;
    ScopedPointer<TextButton> button_show_active_input_l_3_1;
    ScopedPointer<TextButton> button_programm_rename;
    ScopedPointer<Monique_Ui_DualSlider> flt_shape_1;
    ScopedPointer<Monique_Ui_DualSlider> flt_shape_2;
    ScopedPointer<Monique_Ui_DualSlider> flt_shape_3;
    ScopedPointer<TextButton> button_programm_scratch;
    ScopedPointer<TextButton> effect_finalizer_switch2;
    ScopedPointer<Monique_Ui_DualSlider> flt_shape_4;
    ScopedPointer<Label> label_monoplugs;
    ScopedPointer<Monique_Ui_DualSlider> delay3;
    ScopedPointer<Label> label_fx_distortion;
    ScopedPointer<Label> label_reverb;
    ScopedPointer<Label> label_fx_chorus;
    ScopedPointer<Label> label_fx_delay;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Monique_Ui_Mainwindow)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_5A0D36FB903763FA__
