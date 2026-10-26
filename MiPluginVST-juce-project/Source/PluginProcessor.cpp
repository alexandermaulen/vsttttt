#include <juce_audio_processors/juce_audio_processors.h>
// Generado por VST Studio Creator
class Proc:public juce::AudioProcessor{
public:
  Proc():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),apvts(*this,nullptr,"STATE",layout()){}
  static juce::AudioProcessorValueTreeState::ParameterLayout layout(){
  juce::AudioProcessorValueTreeState::ParameterLayout L;
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b0_g",1},"1 Ganancia - Ganancia dB",juce::NormalisableRange<float>(-24.0f,24.0f),0.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b1_d",1},"2 Saturación - Drive dB",juce::NormalisableRange<float>(0.0f,36.0f),6.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b1_m",1},"2 Saturación - Mezcla 0-1",juce::NormalisableRange<float>(0.0f,1.0f),1.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b2_t",1},"3 Compresor - Umbral dB",juce::NormalisableRange<float>(-60.0f,0.0f),-18.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b2_r",1},"3 Compresor - Ratio",juce::NormalisableRange<float>(1.0f,20.0f),4.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b2_a",1},"3 Compresor - Ataque ms",juce::NormalisableRange<float>(0.1f,100.0f),10.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b2_l",1},"3 Compresor - Release ms",juce::NormalisableRange<float>(10.0f,1000.0f),120.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b2_k",1},"3 Compresor - Makeup dB",juce::NormalisableRange<float>(0.0f,24.0f),0.0f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b3_c",1},"4 Limitador - Techo dB",juce::NormalisableRange<float>(-12.0f,0.0f),-0.3f));
  L.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"b3_l",1},"4 Limitador - Release ms",juce::NormalisableRange<float>(10.0f,500.0f),80.0f));
  return L;}
  void prepareToPlay(double s,int)override{sr=s;for(auto&r:st)r[0]=r[1]=0.f;
    st[3][0]=st[3][1]=1.f;
  }
  void releaseResources()override{}
  bool isBusesLayoutSupported(const BusesLayout&l)const override{return l.getMainOutputChannelSet()==l.getMainInputChannelSet()&&!l.getMainOutputChannelSet().isDisabled();}
  void processBlock(juce::AudioBuffer<float>&buf,juce::MidiBuffer&)override{
    juce::ScopedNoDenormals den;
    const float g0=apvts.getRawParameterValue("b0_g")->load();
    const float gg0=juce::Decibels::decibelsToGain(g0);
    const float d1=apvts.getRawParameterValue("b1_d")->load();
    const float m1=apvts.getRawParameterValue("b1_m")->load();
    const float dr1=juce::Decibels::decibelsToGain(d1);
    const float t2=apvts.getRawParameterValue("b2_t")->load();
    const float r2=apvts.getRawParameterValue("b2_r")->load();
    const float a2=apvts.getRawParameterValue("b2_a")->load();
    const float l2=apvts.getRawParameterValue("b2_l")->load();
    const float k2=apvts.getRawParameterValue("b2_k")->load();
    const float ca2=std::exp(-1.f/(0.001f*a2*(float)sr)),cr2=std::exp(-1.f/(0.001f*l2*(float)sr));
    const float c3=apvts.getRawParameterValue("b3_c")->load();
    const float l3=apvts.getRawParameterValue("b3_l")->load();
    const float cl3=juce::Decibels::decibelsToGain(c3),lr3=std::exp(-1.f/(0.001f*l3*(float)sr));
    for(int ch=0;ch<juce::jmin(2,buf.getNumChannels());++ch){auto*d=buf.getWritePointer(ch);
      for(int n=0;n<buf.getNumSamples();++n){float x=d[n];
        x*=gg0;
        x=x*(1.f-m1)+m1*std::tanh(x*dr1)/std::tanh(dr1);
        {auto&e=st[2][ch];float lv=std::abs(x);float co=lv>e?ca2:cr2;e=co*e+(1.f-co)*lv;float gr=juce::jmax(0.f,juce::Decibels::gainToDecibels(e,-100.f)-t2)*(1.f-1.f/r2);x*=juce::Decibels::decibelsToGain(k2-gr);}
        {auto&e=st[3][ch];float ne=juce::jmin(1.f,cl3/(std::abs(x)+1e-9f));e=ne<e?ne:lr3*e+(1.f-lr3)*ne;x=juce::jlimit(-cl3,cl3,x*e);}
        d[n]=x;}}}
  juce::AudioProcessorEditor*createEditor()override{return new juce::GenericAudioProcessorEditor(*this);}
  bool hasEditor()const override{return true;}
  const juce::String getName()const override{return "MiPluginVST";}
  bool acceptsMidi()const override{return false;} bool producesMidi()const override{return false;}
  double getTailLengthSeconds()const override{return 0;}
  int getNumPrograms()override{return 1;} int getCurrentProgram()override{return 0;} void setCurrentProgram(int)override{}
  const juce::String getProgramName(int)override{return {};} void changeProgramName(int,const juce::String&)override{}
  void getStateInformation(juce::MemoryBlock&d)override{if(auto x=apvts.copyState().createXml())copyXmlToBinary(*x,d);}
  void setStateInformation(const void*p,int n)override{if(auto x=getXmlFromBinary(p,n))apvts.replaceState(juce::ValueTree::fromXml(*x));}
  juce::AudioProcessorValueTreeState apvts;
private:
  double sr=44100;float st[4][2]={};
};
juce::AudioProcessor*JUCE_CALLTYPE createPluginFilter(){return new Proc();}
