
const int num_pal_entries = 256;

const float palette_data[num_pal_entries+1][3] =
{
  { 0.0000f, 0.0000f, 0.4650f }, { 0.0000f, 0.0012f, 0.4760f }, { 0.0000f, 0.0024f, 0.4870f }, { 0.0000f, 0.0036f, 0.4980f }, 
  { 0.0000f, 0.0048f, 0.5089f }, { 0.0000f, 0.0060f, 0.5199f }, { 0.0000f, 0.0072f, 0.5309f }, { 0.0000f, 0.0084f, 0.5419f }, 
  { 0.0000f, 0.0096f, 0.5529f }, { 0.0000f, 0.0108f, 0.5639f }, { 0.0000f, 0.0120f, 0.5749f }, { 0.0000f, 0.0132f, 0.5858f }, 
  { 0.0000f, 0.0143f, 0.5968f }, { 0.0000f, 0.0170f, 0.6078f }, { 0.0000f, 0.0215f, 0.6188f }, { 0.0000f, 0.0259f, 0.6298f }, 
  { 0.0000f, 0.0303f, 0.6408f }, { 0.0000f, 0.0348f, 0.6511f }, { 0.0000f, 0.0392f, 0.6611f }, { 0.0000f, 0.0471f, 0.6710f }, 
  { 0.0000f, 0.0557f, 0.6810f }, { 0.0000f, 0.0643f, 0.6910f }, { 0.0000f, 0.0730f, 0.7009f }, { 0.0000f, 0.0816f, 0.7109f }, 
  { 0.0000f, 0.0902f, 0.7208f }, { 0.0000f, 0.0988f, 0.7308f }, { 0.0000f, 0.1074f, 0.7407f }, { 0.0000f, 0.1163f, 0.7507f }, 
  { 0.0000f, 0.1272f, 0.7607f }, { 0.0000f, 0.1380f, 0.7706f }, { 0.0000f, 0.1489f, 0.7814f }, { 0.0000f, 0.1597f, 0.7927f }, 
  { 0.0000f, 0.1706f, 0.8041f }, { 0.0000f, 0.1814f, 0.8154f }, { 0.0000f, 0.1923f, 0.8268f }, { 0.0000f, 0.2031f, 0.8381f }, 
  { 0.0000f, 0.2140f, 0.8495f }, { 0.0000f, 0.2248f, 0.8608f }, { 0.0000f, 0.2357f, 0.8722f }, { 0.0000f, 0.2471f, 0.8836f }, 
  { 0.0000f, 0.2590f, 0.8949f }, { 0.0000f, 0.2708f, 0.9046f }, { 0.0000f, 0.2827f, 0.9129f }, { 0.0000f, 0.2945f, 0.9211f }, 
  { 0.0000f, 0.3064f, 0.9294f }, { 0.0000f, 0.3183f, 0.9377f }, { 0.0000f, 0.3308f, 0.9460f }, { 0.0000f, 0.3444f, 0.9543f }, 
  { 0.0000f, 0.3579f, 0.9626f }, { 0.0000f, 0.3714f, 0.9705f }, { 0.0000f, 0.3849f, 0.9756f }, { 0.0000f, 0.3984f, 0.9807f }, 
  { 0.0000f, 0.4120f, 0.9858f }, { 0.0000f, 0.4277f, 0.9909f }, { 0.0000f, 0.4440f, 0.9960f }, { 0.0000f, 0.4604f, 1.0000f }, 
  { 0.0000f, 0.4767f, 1.0000f }, { 0.0000f, 0.4931f, 1.0000f }, { 0.0000f, 0.5094f, 1.0000f }, { 0.0000f, 0.5257f, 1.0000f }, 
  { 0.0000f, 0.5421f, 1.0000f }, { 0.0000f, 0.5584f, 1.0000f }, { 0.0000f, 0.5748f, 1.0000f }, { 0.0000f, 0.5911f, 1.0000f }, 
  { 0.0000f, 0.6074f, 1.0000f }, { 0.0000f, 0.6239f, 1.0000f }, { 0.0000f, 0.6409f, 1.0000f }, { 0.0000f, 0.6579f, 1.0000f }, 
  { 0.0000f, 0.6748f, 1.0000f }, { 0.0000f, 0.6918f, 1.0000f }, { 0.0000f, 0.7087f, 1.0000f }, { 0.0000f, 0.7257f, 1.0000f }, 
  { 0.0000f, 0.7427f, 1.0000f }, { 0.0000f, 0.7596f, 1.0000f }, { 0.0000f, 0.7766f, 1.0000f }, { 0.0000f, 0.7919f, 1.0000f }, 
  { 0.0000f, 0.8057f, 1.0000f }, { 0.0000f, 0.8194f, 1.0000f }, { 0.0000f, 0.8332f, 1.0000f }, { 0.0000f, 0.8469f, 1.0000f }, 
  { 0.0000f, 0.8606f, 1.0000f }, { 0.0000f, 0.8744f, 1.0000f }, { 0.0000f, 0.8872f, 1.0000f }, { 0.0000f, 0.8992f, 0.9990f }, 
  { 0.0000f, 0.9113f, 0.9960f }, { 0.0000f, 0.9234f, 0.9929f }, { 0.0000f, 0.9355f, 0.9898f }, { 0.0000f, 0.9465f, 0.9867f }, 
  { 0.0000f, 0.9536f, 0.9819f }, { 0.0000f, 0.9607f, 0.9749f }, { 0.0000f, 0.9678f, 0.9679f }, { 0.0000f, 0.9749f, 0.9609f }, 
  { 0.0000f, 0.9820f, 0.9511f }, { 0.0000f, 0.9866f, 0.9408f }, { 0.0000f, 0.9894f, 0.9306f }, { 0.0000f, 0.9922f, 0.9203f }, 
  { 0.0000f, 0.9950f, 0.9100f }, { 0.0000f, 0.9978f, 0.8950f }, { 0.0000f, 1.0000f, 0.8800f }, { 0.0000f, 1.0000f, 0.8649f }, 
  { 0.0000f, 1.0000f, 0.8449f }, { 0.0000f, 1.0000f, 0.8224f }, { 0.0000f, 1.0000f, 0.8000f }, { 0.0000f, 1.0000f, 0.7775f }, 
  { 0.0000f, 1.0000f, 0.7551f }, { 0.0001f, 1.0000f, 0.7323f }, { 0.0029f, 1.0000f, 0.7092f }, { 0.0057f, 1.0000f, 0.6861f }, 
  { 0.0085f, 1.0000f, 0.6630f }, { 0.0113f, 1.0000f, 0.6399f }, { 0.0141f, 1.0000f, 0.6168f }, { 0.0169f, 1.0000f, 0.5913f }, 
  { 0.0196f, 1.0000f, 0.5656f }, { 0.0266f, 1.0000f, 0.5398f }, { 0.0342f, 1.0000f, 0.5141f }, { 0.0418f, 1.0000f, 0.4883f }, 
  { 0.0494f, 1.0000f, 0.4626f }, { 0.0586f, 1.0000f, 0.4371f }, { 0.0721f, 1.0000f, 0.4116f }, { 0.0856f, 1.0000f, 0.3860f }, 
  { 0.0991f, 1.0000f, 0.3605f }, { 0.1127f, 1.0000f, 0.3349f }, { 0.1262f, 1.0000f, 0.3094f }, { 0.1397f, 1.0000f, 0.2855f }, 
  { 0.1581f, 1.0000f, 0.2625f }, { 0.1795f, 1.0000f, 0.2395f }, { 0.2010f, 1.0000f, 0.2165f }, { 0.2225f, 1.0000f, 0.1936f }, 
  { 0.2440f, 1.0000f, 0.1706f }, { 0.2665f, 1.0000f, 0.1476f }, { 0.2901f, 1.0000f, 0.1246f }, { 0.3137f, 1.0000f, 0.1017f }, 
  { 0.3373f, 1.0000f, 0.0912f }, { 0.3609f, 1.0000f, 0.0818f }, { 0.3845f, 1.0000f, 0.0723f }, { 0.4081f, 1.0000f, 0.0629f }, 
  { 0.4317f, 1.0000f, 0.0534f }, { 0.4553f, 1.0000f, 0.0440f }, { 0.4789f, 1.0000f, 0.0345f }, { 0.5025f, 1.0000f, 0.0251f }, 
  { 0.5261f, 1.0000f, 0.0220f }, { 0.5500f, 1.0000f, 0.0189f }, { 0.5750f, 1.0000f, 0.0159f }, { 0.6000f, 1.0000f, 0.0128f }, 
  { 0.6249f, 1.0000f, 0.0098f }, { 0.6499f, 1.0000f, 0.0067f }, { 0.6749f, 1.0000f, 0.0037f }, { 0.6999f, 1.0000f, 0.0006f }, 
  { 0.7249f, 1.0000f, 0.0000f }, { 0.7499f, 1.0000f, 0.0000f }, { 0.7748f, 1.0000f, 0.0000f }, { 0.7998f, 1.0000f, 0.0000f }, 
  { 0.8235f, 1.0000f, 0.0000f }, { 0.8417f, 1.0000f, 0.0000f }, { 0.8600f, 1.0000f, 0.0000f }, { 0.8782f, 1.0000f, 0.0000f }, 
  { 0.8946f, 1.0000f, 0.0000f }, { 0.9076f, 1.0000f, 0.0000f }, { 0.9206f, 1.0000f, 0.0000f }, { 0.9336f, 1.0000f, 0.0000f }, 
  { 0.9439f, 1.0000f, 0.0000f }, { 0.9516f, 1.0000f, 0.0000f }, { 0.9592f, 1.0000f, 0.0000f }, { 0.9668f, 1.0000f, 0.0000f }, 
  { 0.9745f, 1.0000f, 0.0000f }, { 0.9821f, 1.0000f, 0.0000f }, { 0.9863f, 1.0000f, 0.0000f }, { 0.9883f, 0.9991f, 0.0000f }, 
  { 0.9903f, 0.9976f, 0.0000f }, { 0.9923f, 0.9961f, 0.0000f }, { 0.9943f, 0.9946f, 0.0000f }, { 0.9964f, 0.9931f, 0.0000f }, 
  { 0.9984f, 0.9916f, 0.0000f }, { 1.0000f, 0.9901f, 0.0000f }, { 1.0000f, 0.9839f, 0.0000f }, { 1.0000f, 0.9773f, 0.0000f }, 
  { 1.0000f, 0.9708f, 0.0000f }, { 1.0000f, 0.9643f, 0.0000f }, { 1.0000f, 0.9567f, 0.0000f }, { 1.0000f, 0.9470f, 0.0000f }, 
  { 1.0000f, 0.9372f, 0.0000f }, { 1.0000f, 0.9274f, 0.0000f }, { 1.0000f, 0.9177f, 0.0000f }, { 1.0000f, 0.9079f, 0.0000f }, 
  { 1.0000f, 0.8981f, 0.0000f }, { 1.0000f, 0.8855f, 0.0000f }, { 1.0000f, 0.8716f, 0.0000f }, { 1.0000f, 0.8576f, 0.0000f }, 
  { 1.0000f, 0.8437f, 0.0000f }, { 1.0000f, 0.8297f, 0.0000f }, { 1.0000f, 0.8155f, 0.0000f }, { 1.0000f, 0.8005f, 0.0000f }, 
  { 1.0000f, 0.7856f, 0.0000f }, { 1.0000f, 0.7707f, 0.0000f }, { 1.0000f, 0.7557f, 0.0000f }, { 1.0000f, 0.7408f, 0.0000f }, 
  { 1.0000f, 0.7258f, 0.0000f }, { 1.0000f, 0.7109f, 0.0000f }, { 1.0000f, 0.6960f, 0.0000f }, { 1.0000f, 0.6794f, 0.0000f }, 
  { 1.0000f, 0.6618f, 0.0000f }, { 1.0000f, 0.6442f, 0.0000f }, { 1.0000f, 0.6265f, 0.0000f }, { 1.0000f, 0.6089f, 0.0000f }, 
  { 1.0000f, 0.5913f, 0.0000f }, { 1.0000f, 0.5737f, 0.0000f }, { 1.0000f, 0.5560f, 0.0000f }, { 1.0000f, 0.5384f, 0.0000f }, 
  { 1.0000f, 0.5208f, 0.0000f }, { 1.0000f, 0.5028f, 0.0000f }, { 1.0000f, 0.4820f, 0.0000f }, { 1.0000f, 0.4611f, 0.0000f }, 
  { 0.9988f, 0.4402f, 0.0000f }, { 0.9948f, 0.4194f, 0.0000f }, { 0.9907f, 0.3985f, 0.0000f }, { 0.9866f, 0.3777f, 0.0000f }, 
  { 0.9826f, 0.3568f, 0.0000f }, { 0.9785f, 0.3359f, 0.0000f }, { 0.9739f, 0.3151f, 0.0000f }, { 0.9661f, 0.2942f, 0.0000f }, 
  { 0.9583f, 0.2733f, 0.0000f }, { 0.9504f, 0.2570f, 0.0000f }, { 0.9426f, 0.2415f, 0.0000f }, { 0.9347f, 0.2260f, 0.0000f }, 
  { 0.9231f, 0.2104f, 0.0000f }, { 0.9115f, 0.1949f, 0.0000f }, { 0.8999f, 0.1794f, 0.0000f }, { 0.8883f, 0.1639f, 0.0000f }, 
  { 0.8767f, 0.1484f, 0.0000f }, { 0.8651f, 0.1329f, 0.0000f }, { 0.8535f, 0.1174f, 0.0000f }, { 0.8419f, 0.1019f, 0.0000f }, 
  { 0.8288f, 0.0864f, 0.0000f }, { 0.8154f, 0.0709f, 0.0000f }, { 0.8020f, 0.0553f, 0.0000f }, { 0.7885f, 0.0398f, 0.0000f }, 
  { 0.7751f, 0.0243f, 0.0000f }, { 0.7617f, 0.0088f, 0.0000f }, { 0.7483f, 0.0000f, 0.0000f }, { 0.7348f, 0.0000f, 0.0000f }, 
  { 0.7210f, 0.0000f, 0.0000f }, { 0.7068f, 0.0000f, 0.0000f }, { 0.6927f, 0.0000f, 0.0000f }, { 0.6786f, 0.0000f, 0.0000f }, 
  { 0.6645f, 0.0000f, 0.0000f }, { 0.6503f, 0.0000f, 0.0000f }, { 0.6362f, 0.0000f, 0.0000f }, { 0.6221f, 0.0000f, 0.0000f }, 
  { 0.6080f, 0.0000f, 0.0000f }, { 0.5939f, 0.0000f, 0.0000f }, { 0.5797f, 0.0000f, 0.0000f }, { 0.5656f, 0.0000f, 0.0000f }, 
  { 0.5515f, 0.0000f, 0.0000f }, { 0.5374f, 0.0000f, 0.0000f }, { 0.5232f, 0.0000f, 0.0000f }, { 0.5091f, 0.0000f, 0.0000f }, 
  { 0.4950f, 0.0000f, 0.0000f }
};

