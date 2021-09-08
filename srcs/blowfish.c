#include "woody_woodpacker.h"

uint32_t original_p[18] = {
    0x886a3f24, 0xd308a385, 0x2e8a1913, 0x44737003,
    0x223809a4, 0xd0319f29, 0x98fa2e08, 0x896c4eec,
    0xe6212845, 0x7713d038, 0xcf6654be, 0x6c0ce934,
    0xb729acc0, 0xdd507cc9, 0xb5d5843f, 0x170947b5,
    0xd9d51692, 0x1bfb7989
};

uint32_t original_s[4][256] = {
    {
        0xa60b31d1, 0xacb5df98, 0xdb72fd2f, 0xb7df1ad0,
        0xedafe1b8, 0x967e266a, 0x45907cba, 0x997f2cf1,
        0x4799a124, 0xf76c91b3, 0xe2f20108, 0x16fc8e85,
        0xd8206963, 0x694e5771, 0xa3fe58a4, 0x7e3d93f4,
        0x8f74950d, 0x58b68e72, 0x58cd8b71, 0xee4a1582,
        0x1da4547b, 0xb5595ac2, 0x39d5309c, 0x1360f22a,
        0x23b0d1c5, 0xf0856028, 0x187941ca, 0xef38dbb8,
        0xb0dc798e, 0xe183a60, 0x8b0e9e6c, 0x3e8a1eb0,
        0xc17715d7, 0x274b31bd, 0xda2faf78, 0x605c6055,
        0xf32555e6, 0x94ab55aa, 0x62984857, 0x4014e863,
        0x6a39ca55, 0xb610ab2a, 0x345cccb4, 0xcee84111,
        0xaf8654a1, 0x93e9727c, 0x1114eeb3, 0x2abc6f63,
        0x5dc5a92b, 0xf6311874, 0x163e5cce, 0x1e93879b,
        0x33bad6af, 0x5ccf246c, 0x8153327a, 0x77869528,
        0x98488f3b, 0xafb94b6b, 0x1be8bfc4, 0x93212866,
        0xcc09d861, 0x91a921fb, 0x60ac7c48, 0x3280ec5d,
        0x5d5d84ef, 0xb17585e9, 0x22326dc, 0x881b65eb,
        0x813e8923, 0xc5ac96d3, 0xf36f6d0f, 0x3942f483,
        0x82440b2e, 0x42084a4, 0x4af0c869, 0x5e9b1f9e,
        0x4268c621, 0x9a6ce9f6, 0x619c0c67, 0xf088d3ab,
        0xd2a0516a, 0x682f54d8, 0x28a70f96, 0xa33351ab,
        0x6c0bef6e, 0xe43b7a13, 0x50f03bba, 0x982afb7e,
        0x1d65f1a1, 0x7601af39, 0x3e59ca66, 0x880e4382,
        0x1986ee8c, 0xb49f6f45, 0xc3a5847d, 0xbe5e8b3b,
        0xd8756fe0, 0x7320c185, 0x9f441a40, 0xa66ac156,
        0x62aad34e, 0x6773f36, 0x72dffe1b, 0x3d029b42,
        0x24d7d037, 0x48120ad0, 0xd3ea0fdb, 0x9bc0f149,
        0xc9725307, 0x7b1b9980, 0xd879d425, 0xf7dee8f6,
        0x1a50fee3, 0x3b4c79b6, 0xbde06c97, 0xba06c004,
        0xb64fa9c1, 0xc4609f40, 0xc29e5c5e, 0x63246a19,
        0xaf6ffb68, 0xb5536c3e, 0xebb23913, 0x6fec523b,
        0x1f51fc6d, 0x2c95309b, 0x444581cc, 0x9bd5eaf,
        0x4d0e3be, 0xfd4a33de, 0x7280f66, 0xb34b2e19,
        0x57a8cbc0, 0xf74c845, 0x395f0bd2, 0xdbfbd3b9,
        0xbdc07955, 0xa32601a, 0xc600a1d6, 0x79722c40,
        0xfe259f67, 0xcca31ffb, 0xf8e9a58e, 0xf82232db,
        0xdf16753c, 0x156b61fd, 0xc81e502f, 0xab5205ad,
        0xfab53d32, 0x608723fd, 0x487b3153, 0x82df003e,
        0xbb575c9e, 0xa08c6fca, 0x2e56871a, 0xdb6917df,
        0xf6a842d5, 0xc3ff7e28, 0xc63267ac, 0x73554f8c,
        0xb0275b69, 0xc858cabb, 0x5da3ffe1, 0xa011f0b8,
        0x983dfa10, 0xb88321fd, 0x6cb5fc4a, 0x5bd3d12d,
        0x79e4539a, 0x6545f8b6, 0xbc498ed2, 0x9097fb4b,
        0xdaf2dde1, 0x337ecba4, 0x4113fb62, 0xe8c6e4ce,
        0xdaca20ef, 0x14c7736, 0xfe9e7ed0, 0xb41ff12b,
        0x4ddadb95, 0x989190ae, 0x718eadea, 0xa0d5936b,
        0xd0d18ed0, 0xe025c7af, 0x2f5b3c8e, 0xb794758e,
        0xfbe2f68f, 0x642b12f2, 0x12b88888, 0x1cf00d90,
        0xa05ead4f, 0x1cc38f68, 0x91f1cfd1, 0xadc1a8b3,
        0x18222f2f, 0x77170ebe, 0xfe2d75ea, 0xa11f028b,
        0xfcca0e5, 0xe8746fb5, 0xd6f3ac18, 0x99e289ce,
        0xe04fa8b4, 0xb7e013fd, 0x813bc47c, 0xd9a8add2,
        0x66a25f16, 0x5779580, 0x1473cc93, 0x77141a21,
        0x6520ade6, 0x86fab577, 0xf54254c7, 0xcf359dfb,
        0xcafcdeb, 0xa0893e7b, 0xd31b41d6, 0x497e1eae,
        0x2d0e2500, 0x5eb37120, 0xbb006822, 0xafe0b857,
        0x9b366424, 0x1eb909f0, 0x1d916355, 0xaaa6df59,
        0x8943c178, 0x7f535ad9, 0xa25b7d20, 0xc5b9e502,
        0x76032683, 0xa9cf9562, 0x6819c811, 0x414a734e,
        0xca2d47b3, 0x4aa9147b, 0x5200511b, 0x1529539a,
        0x3f570fd6, 0xe4c69bbc, 0x76a4602b, 0x74e681,
        0xb56fba08, 0x1fe91b57, 0x6bec96f2, 0x15d90d2a,
        0x216563b6, 0xb6f9b9e7, 0x2e0534ff, 0x645685c5,
        0x5d2db053, 0xa18f9fa9, 0x9947ba08, 0x6a07856e
    },
    {
        0xe9707a4b, 0x4429b3b5, 0x2e0975db, 0x232619c4,
        0xb0a66ead, 0x7ddfa749, 0xb860ee9c, 0x66b2ed8f,
        0x718caaec, 0xff179a69, 0x6c526456, 0xe19eb1c2,
        0xa5023619, 0x294c0975, 0x401359a0, 0x3e3a18e4,
        0x9a98543f, 0x659d425b, 0xd6e48f6b, 0xd63ff799,
        0x79cd2a1, 0xf530e8ef, 0xe6382d4d, 0xc15d25f0,
        0x8620dd4c, 0x26eb7084, 0xc6e98263, 0x5ecc1e02,
        0x3f6b6809, 0xc9efba3e, 0x1418973c, 0xa1706a6b,
        0x84357f68, 0x86e2a052, 0x5539cb7, 0x370750aa,
        0x1c84073e, 0x5caede7f, 0xec447d8e, 0xb8f21657,
        0x37da3ab0, 0xd0c50f0, 0x41f1cf0, 0xffb30002,
        0x1af50cae, 0xb274b53c, 0x587a8325, 0xbd2109dc,
        0xf91391d1, 0xf62fa97c, 0x73473294, 0x147f522,
        0x81e5e53a, 0xdcdac237, 0x3476b5c8, 0xa7ddf39a,
        0x466144a9, 0xe03d00f, 0x3ec7c8ec, 0x411e75a4,
        0x99cd38e2, 0x2f0eea3b, 0xa1bb8032, 0x31b33e18,
        0x388b544e, 0x8b96d4f, 0x30d426f, 0xbf040af6,
        0x9012b82c, 0x797c9724, 0x72b07956, 0xaf89afbc,
        0x1f779ade, 0x100893d9, 0x12ae8bb3, 0x2e3fcfdc,
        0x1f721255, 0x24716b2e, 0xe6dd1a50, 0x87cd849f,
        0x1847587a, 0x17da0874, 0xbc9a9fbc, 0x8c7d4be9,
        0x3aec7aec, 0xfa1d85db, 0x66430963, 0xd2c364c4,
        0x47181cef, 0x8d91532, 0x373b43dd, 0x16bac224,
        0x434da112, 0x51c4652a, 0x2009450, 0xdde43a13,
        0x9ef8df71, 0x554e3110, 0xd677ac81, 0x9b19115f,
        0xf1563504, 0x6bc7a3d7, 0x3b18113c, 0x9a52459,
        0xede68ff2, 0xfafbf197, 0x2cbfba9e, 0x6e3c151e,
        0x7045e386, 0xb16fe9ea, 0xa5e0e86, 0xb32a3e5a,
        0x1ce71f77, 0xfa063d4e, 0xb9dc6529, 0xf1de799,
        0xd6893e80, 0x25c86652, 0x78c94c2e, 0x6ab3109c,
        0xba0e15c6, 0x78eae294, 0x533cfca5, 0xf42d0a1e,
        0xa74ef7f2, 0x3d2b1d36, 0xf263919, 0x6079c219,
        0x8a72352, 0xb61213f7, 0x6efeadeb, 0x661fc3ea,
        0x9545bce3, 0x83c87ba6, 0xd1377fb1, 0x28ff8c01,
        0xefdd32c3, 0xa55a6cbe, 0x85215865, 0x298ab68,
        0xfa5ceee, 0x3b952fdb, 0xad7def2a, 0x842f6e5b,
        0x28b62115, 0x70610729, 0x7547ddec, 0x10159f61,
        0x30a8cc13, 0x96bd61eb, 0x1efe3403, 0xcf6303aa,
        0x905c73b5, 0x39a2704c, 0xb9e9ed5, 0x14deaacb,
        0xbc86ccee, 0xa72c6260, 0xab5cab9c, 0x6e84f3b2,
        0xaf1e8b64, 0xcaf0bd19, 0xb96923a0, 0x50bb5a65,
        0x325a6840, 0xb3b42a3c, 0xd5e99e31, 0xf7b821c0,
        0x190b549b, 0x99a05f87, 0x7e99f795, 0xa87d3d62,
        0x9a8837f8, 0x772de397, 0x5f93ed11, 0x81126816,
        0x2988350e, 0xd61fe6c7, 0xa1dfde96, 0x99ba5878,
        0xa584f557, 0x6372221b, 0xffc3839b, 0x9646c21a,
        0xeb0ab3cd, 0x54302e53, 0xe448d98f, 0x2831bc6d,
        0xeff2eb58, 0xeaffc634, 0x61ed28fe, 0x733c7cee,
        0xd9144a5d, 0xe3b764e8, 0x145d1042, 0xe0133e20,
        0xb6e2ee45, 0xeaabaaa3, 0x154f6cdb, 0xd04fcbfa,
        0x42f442c7, 0xb5bb6aef, 0x1d3b4f65, 0x521cd41,
        0x9e791ed8, 0xc74d8586, 0x6a474be4, 0x5062813d,
        0xf2a162cf, 0x46268d5b, 0xa08388fc, 0xa3b6c7c1,
        0xc324157f, 0x9274cb69, 0xb8a8447, 0x85b29256,
        0xbf5b09, 0x9d4819ad, 0x74b16214, 0xe8223,
        0x2a8d4258, 0xeaf5550c, 0x3ef4ad1d, 0x61703f23,
        0x92f07233, 0x417e938d, 0xf1ec5fd6, 0xdb3b226c,
        0x5937de7c, 0x6074eecb, 0xa7f28540, 0x6e3277ce,
        0x848007a6, 0x9e50f819, 0x55d8efe8, 0x3597d961,
        0xaaa769a9, 0xc2060cc5, 0xfcab045a, 0xdcca0b80,
        0x2e7a449e, 0x843445c3, 0x567d5fd, 0xc99e1e0e,
        0xd3db73db, 0xcd885510, 0x79da5f67, 0x404367e3,
        0x6534c4c5, 0xd8383e71, 0x9ef8283d, 0x20ff6df1,
        0xe7213e15, 0x4a3db08f, 0x2b9fe3e6, 0xf7ad83db
    },
    {
        0x685a3de9, 0xf7408194, 0x1c264cf6, 0x34296994,
        0xf7201541, 0xf7d40276, 0x2e6bf4bc, 0x6800a2d4,
        0x712408d4, 0x6af42033, 0xb7d4b743, 0xaf610050,
        0x2ef6391e, 0x46452497, 0x744f2114, 0x40888bbf,
        0x1dfc954d, 0xaf91b596, 0xd3ddf470, 0x452fa066,
        0xec09bcbf, 0x8597bd03, 0xd06dac7f, 0x485cb31,
        0xb327eb96, 0x4139fd55, 0xe64725da, 0x9a0acaab,
        0x25785028, 0xf4290453, 0xda862c0a, 0xfb6db6e9,
        0x6214dc68, 0x6948d7, 0xa4c00e68, 0xee8da127,
        0xa2fe3f4f, 0x8cad87e8, 0x6e08cb5, 0xb6d6f47a,
        0x7c1eceaa, 0xec5f37d3, 0x99a378ce, 0x422a6b40,
        0x359efe20, 0xb985f3d9, 0xabd739ee, 0x8b4e123b,
        0xf7fac91d, 0x56186d4b, 0x3166a326, 0xb297e3ea,
        0x74fa6e3a, 0x32435bdd, 0xf7e74168, 0xfb2078ca,
        0x4ef50afb, 0x97b3fed8, 0xac564045, 0x279548ba,
        0x3a3a5355, 0x878d8320, 0xb7a96bfe, 0x4b9596d0,
        0xbc67a855, 0x589a15a1, 0x6329a9cc, 0x33dbe199,
        0x564a2aa6, 0xf925313f, 0x1c7ef45e, 0x7c312990,
        0x2e8f8fd, 0x702f2704, 0x5c15bb80, 0xe32c2805,
        0x4815c195, 0x226dc6e4, 0x3f13c148, 0xdc860fc7,
        0xeec9f907, 0xf1f0441, 0xa4794740, 0x176e885d,
        0xeb515f32, 0xd1c09bd5, 0x8fc1bcf2, 0x64351141,
        0x34787b25, 0x609c2a60, 0xa3e8f8df, 0x1b6c631f,
        0xc2b4120e, 0x9e32e102, 0xd14f66af, 0x1581d1ca,
        0xe095236b, 0xe1923e33, 0x620b243b, 0x22b9beee,
        0xea2b285, 0x990dbae6, 0x8c0c72de, 0x28f7a22d,
        0x457812d0, 0xfd94b795, 0x62087d64, 0xf0f5cce7,
        0x6fa34954, 0xfa487d87, 0x27fd9dc3, 0x1e8d3ef3,
        0x4163470a, 0x74ff2e99, 0xab6e6f3a, 0x37fdf8f4,
        0x60dc12a8, 0xf8ddeba1, 0x4ce11b99, 0xd6b6edb,
        0x10557bc6, 0x372c676d, 0x3bd46527, 0x4e8d0dc,
        0xc70d29f1, 0xa3ff00cc, 0x920f39b5, 0xbed0f69,
        0xfb9f7b66, 0x9c7ddbce, 0xbcf91a0, 0xa35e15d9,
        0x882f13bb, 0x24ad5b51, 0xbf79947b, 0xebd63b76,
        0xb32e3937, 0x795911cc, 0x97e22680, 0x2d312ef4,
        0xa7ad4268, 0x3b2b6ac6, 0xcc4c7512, 0x1cf12e78,
        0x3742126a, 0xe75192b7, 0xe6bba106, 0x5063fb4b,
        0x18106b1a, 0xfaedca11, 0xd8bd253d, 0xc9c3e1e2,
        0x59164244, 0x8613120a, 0x6eec0cd9, 0x2aeaabd5,
        0x4e67af64, 0x5fa886da, 0x88e9bfbe, 0xfec3e464,
        0x5780bc9d, 0x86c0f7f0, 0xf87b7860, 0x4d600360,
        0x4683fdd1, 0xb01f38f6, 0x4ae4577, 0xccfc36d7,
        0x336b4283, 0x71ab1ef0, 0x874180b0, 0x5f5e003c,
        0xbe57a077, 0x24aee8bd, 0x99424655, 0x612e58bf,
        0x8ff4584e, 0xa2fdddf2, 0x38ef74f4, 0xc2bd8987,
        0xc3f96653, 0x748eb3c8, 0x55f275b4, 0xb9d9fc46,
        0x6126eb7a, 0x84df1d8b, 0x790e6a84, 0xe2955f91,
        0x8e596e46, 0x7057b420, 0x9155d58c, 0x4cde02c9,
        0xe1ac0bb9, 0xd00582bb, 0x4862a811, 0x9ea97475,
        0xb6197fb7, 0x9dca9e0, 0xa1092d66, 0x334632c4,
        0x21f5ae8, 0x8cbef009, 0x25a0994a, 0x10fe6e1d,
        0x1d3db91a, 0xdfa4a50b, 0xff286a1, 0x69f16828,
        0x83dab7dc, 0xfe063957, 0x9bcee2a1, 0x527fcd4f,
        0x15e1150, 0xfa8306a7, 0xc4b502a0, 0x27d0e60d,
        0x278cf89a, 0x41863f77, 0x64c60c3, 0xb506a861,
        0x287a17f0, 0xe086f5c0, 0xaa586000, 0x627ddc30,
        0xd79ee611, 0x63ea3823, 0x94ddc253, 0x3416c2c2,
        0x56eecbbb, 0xdeb6bc90, 0xa17dfceb, 0x761d59ce,
        0x9e4056f, 0x88017c4b, 0x3d0a7239, 0x247c927c,
        0x5f72e386, 0xb99d4d72, 0xb45bc11a, 0xfcb89ed3,
        0x785554ed, 0xb5a5fc08, 0xd37c3dd8, 0xc40fad4d,
        0x5eef501e, 0xf8e661b1, 0xd91485a2, 0x3c13516c,
        0xe7c7d56f, 0xc44ee156, 0xcebf2a36, 0x37c8c6dd,
        0x34329ad7, 0x12826392, 0x8efa0e67, 0xe0006040
    },
    {
        0x37ce393a, 0xcff5fad3, 0x3777c2ab, 0x1b2dc55a,
        0x9e67b05c, 0x4237a34f, 0x402782d3, 0xbe9bbc99,
        0x9d8e11d5, 0x15730fbf, 0x7e1c2dd6, 0x7bc400c7,
        0x6b1b8cb7, 0x4590a121, 0xbeb16eb2, 0xb46e366a,
        0x2fab4857, 0x796e94bc, 0xd276a3c6, 0xc8c24965,
        0xeef80f53, 0x7dde8d46, 0x1d0a73d5, 0xc64dd04c,
        0xdbbb3929, 0x5046baa9, 0xe82695ac, 0x4e35ebe,
        0xf0d5faa1, 0x9a512d6a, 0xe28cef63, 0x22ee869a,
        0xb8c289c0, 0xf62e2443, 0xaa031ea5, 0xa4d0f29c,
        0xba61c083, 0x4d6ae99b, 0x5015e58f, 0xd65b64ba,
        0xf9a22628, 0xe13a3aa7, 0x8695a94b, 0xe96255ef,
        0xd3ef2fc7, 0xdaf752f7, 0x696f043f, 0x590afa77,
        0x15a9e480, 0x186b087, 0xade6099b, 0x93e53e3b,
        0x5afd90e9, 0x97d7349e, 0xd9b7f02c, 0x518b2b02,
        0x3aacd596, 0x7da67d01, 0xd63ecfd1, 0x282d7d7c,
        0xcf259f1f, 0x9bb8f2ad, 0x72b4d65a, 0x4cf5885a,
        0x71ac29e0, 0xe6a519e0, 0xfdacb047, 0x9bfa93ed,
        0x8dc4d3e8, 0xcc573b28, 0x2966d5f8, 0x282e1379,
        0x91015f78, 0x556075ed, 0x440e96f7, 0x8c5ed3e3,
        0xd46d0515, 0xba6df488, 0x2561a103, 0xbdf06405,
        0x159eebc3, 0xa257903c, 0xec1a2797, 0x2a073aa9,
        0x9b6d3f1b, 0xf521631e, 0xfb669cf5, 0x19f3dc26,
        0x28d93375, 0xf5fd55b1, 0x82345603, 0xbb3cba8a,
        0x11775128, 0xf8d90ac2, 0x6751ccab, 0x5f92adcc,
        0x5117e84d, 0x8edc3038, 0x62589d37, 0x91f92093,
        0xc2907aea, 0xce7b3efb, 0x64ce2151, 0x32be4f77,
        0x7ee3b6a8, 0x463d29c3, 0x6953de48, 0x80e61364,
        0x1008aea2, 0x24b26ddd, 0xfd2d8569, 0x66210709,
        0xa469ab3, 0xddc04564, 0xcfde6c58, 0xaec8201c,
        0xddf7be5b, 0x408d581b, 0x7f01d2cc, 0xbbe3b46b,
        0x7e6aa2dd, 0x45ff593a, 0x440a353e, 0xd5cdb4bc,
        0xa8ceea72, 0xbb8464fa, 0xae12668d, 0x476f3cbf,
        0x63e49bd2, 0x9e5d2f54, 0x1b77c2ae, 0x70634ef6,
        0x8d0d0e74, 0x57135be7, 0x711672f8, 0x5d7d53af,
        0x8cb4040, 0xcce2b44e, 0x6a46d234, 0x84af1501,
        0x2804b0e1, 0x1d3a9895, 0xb49fb806, 0x48a06ece,
        0x823b3f6f, 0x82ab2035, 0x4b1d1a01, 0xf8277227,
        0xb1601561, 0xdc3f93e7, 0x2b793abb, 0xbd254534,
        0xe13988a0, 0x4b79ce51, 0xb7c9322f, 0xc9ba1fa0,
        0x7ec81ce0, 0xf6d1c7bc, 0xc31101cf, 0xc7aae8a1,
        0x4987901a, 0x9abd4fd4, 0xcbdedad0, 0x38da0ad5,
        0x2ac33903, 0x673691c6, 0x7c31f98d, 0x4f2bb1e0,
        0xb7599ef7, 0x3abbf543, 0xff19d5f2, 0x9c45d927,
        0x2c2297bf, 0x2afce615, 0x71fc910f, 0x2515949b,
        0x6193e5fa, 0xeb9cb6ce, 0x5964a8c2, 0xd1a8ba12,
        0x5e07c1b6, 0xc6a05e3, 0x6550d210, 0x42a403cb,
        0xe6eece0, 0x3bdb9816, 0xbea0984c, 0x64e97832,
        0x32951f9f, 0xdf92d3e0, 0x2b34a0d3, 0x1ef27189,
        0x41740a1b, 0x8c34a34b, 0x2071bec5, 0xd83276c3,
        0x8d9f35df, 0x2e2f999b, 0x476f0be6, 0x1df1e30f,
        0x54da4ce5, 0x91d8da1e, 0xcf7962ce, 0x6f7e3ecd,
        0x66b11816, 0x51d2cfd, 0xc5d28f84, 0x9922fbf6,
        0x57f323f5, 0x237632a6, 0x3135a893, 0x2cdcc56,
        0x6281f0ac, 0xb5eb755a, 0x9736166e, 0xcc73d288,
        0x926296de, 0xd049b981, 0x1b90504c, 0x1456c671,
        0xbdc7c6e6, 0xa147a32, 0x6d0e145, 0x9a7bf2c3,
        0xfd53aac9, 0xfa862, 0xe2bf25bb, 0xf6d2bd35,
        0x5691271, 0x220204b2, 0x7ccfcbb6, 0x2b9c76cd,
        0xc03e1153, 0xd3e34016, 0x60bdab38, 0xf0ad4725,
        0x9c2038ba, 0x76ce46f7, 0xc5a1af77, 0x60607520,
        0x4efecb85, 0xd88de88a, 0xb0f9aa7a, 0x7eaaf94c,
        0x5cc24819, 0x8c8afb02, 0xe46ac301, 0xf9e1ebd6,
        0x69f8d490, 0xa0de5ca6, 0x2d25093f, 0x9fe608c2,
        0x32614eb7, 0x5be277ce, 0xe3df8f57, 0xe672c33a
    }
};

/* Iterative Function to calculate (x^y)%p in O(log y) */
// int power(long long x, unsigned int y, int p)
// {
//     int res = 1;     // Initialize result

//     if (y == 0) return 1; // In case y is 0;
    
//     x = x % p; // Update x if it is more than or
//                 // equal to p
  
//     if (x == 0) return 0; // In case x is divisible by p;
 
//     while (y > 0)
//     {
//         // If y is odd, multiply x with result
//         if (y & 1)
//             res = (res * x) % p;
 
//         // y must be even now
//         y = y >> 1; // y = y/2
//         x = (x * x) % p;
//         fprintf(stderr, "x: %lld, y: %u\n", x, y);
//     }
//     return res;
// }

// double get_floating_part(double n)
// {
//     if (n < 0.0)
//         return n - ceil(n);
//     return n - floor(n);
// }

// double    _sigma(int n, int a)
// {
//     double res = 0;
//     // double nom = 1. / 16.;
//     double add = 1.;

//     for (int k = 0; k <= n; k++)
//     {
//         // fprintf(stderr, "expmod of 16 ^ (%d - %d) %% %d = %d\n", n, k, (8 * k + a), power(16, n - k, (8 * k + a)));
//         // fprintf(stderr, "power %d %d %d\n", 16, n - k, (8 * k + a));
//         add = ((double)power(16, n - k, (8 * k + a)) / (double)(8 * k + a));
//         fprintf(stderr, "ok!: %d %lf %lf \n", power(16, n - k, (8 * k + a)), (double)(8 * k + a), add);
//         // fprintf(stderr, "------%d: %lf\n", a, res);
//         // if (n - k == 1)
//         //     exit(0);
//     fprintf(stderr, "fpart of %f: %f\n", add, get_floating_part(add));

//         res += get_floating_part(add);
//     fprintf(stderr, "fpart of %f: %f\n", res, get_floating_part(res));

//         res = get_floating_part(res);
//         // res -= (int)res;
//         // fprintf(stderr, "------%d: %lf\n", a, res);
//     }
//     // add = 1;
//     // for (int k = n + 1; add > 0.0000001; k++)
//     // {
//     //     nom = pow(16., (double)(n - k));
//     //     add = nom / (double)(8. * k + a);

//     //     // fprintf(stderr, "add:%lf\n", add);
//     //     res += add;
//     // }
//     // fprintf(stderr, "fpart of %f: %f\n", -1.2, get_floating_part(-1.2));
//     // exit(0);
//     return get_floating_part(res);
// }
//243f6a8885a308d313198a2e03707344a4093822299f31d0082efa98ec4e6c89452821e638d01377be5466cf34e90c6cc0ac29b7c97c50dd3f84d5b5b54709179216d5d98979fb1bd1310ba698dfb5ac2ffd72dbd01adfb7b8e1afed6a267e96ba7c9045f12c7f9924a19947b3916cf70801f2e2858efc16636920d871574e69a458fea3f4933d7e0d95748f728eb658718bcd5882154aee7b54a41dc25a59b59c30d5392af26013c5d1b023286085f0ca417918b8db38ef8e79dcb0603a180e6c9e0e8bb01e8a3ed71577c1bd314b2778af2fda55605c60e65525f3aa55ab945748986263e8144055ca396a2aab10b6b4cc5c341141e8cea15486af7c72e993b3ee1411636fbc2a2ba9c55d741831f6ce5c3e169b87931eafd6ba336c24cf5c7a325381289586773b8f48986b4bb9afc4bfe81b6628219361d809ccfb21a991487cac605dec8032ef845d5de98575b1dc262302eb651b8823893e81d396acc50f6d6ff383f442392e0b4482a484200469c8f04a9e1f9b5e21c66842f6e96c9a670c9c61abd388f06a51a0d2d8542f68960fa728ab5133a36eef0b6c137a3be4ba3bf0507efb2a98a1f1651d39af017666ca593e82430e888cee8619456f9fb47d84a5c33b8b5ebee06f75d885c12073401a449f56c16aa64ed3aa62363f77061bfedf72429b023d37d0d724d00a1248db0fead3
// uint32_t     _bbp_getnth_term(int n)
// {
//     // fprintf(stderr, "sigma: %lf %lf\n", sigma(n, 1), _sigma(n, 1));
//     // exit(0);
//     double res = 4. * sigma(n, 1) - 2. * sigma(n, 4) - sigma(n, 5) - sigma(n, 6);
//     // res = fabs(res);
//     res = get_floating_part(res);
//     if (res < 0)
//         res += 1.;

//     return (int)(16 * fabs(get_floating_part(res)));

// }

// void    uint32_swap(uint32_t *a, uint32_t *b)
// {
//     *a ^= *b;
//     *b ^= *a;
//     *a ^= *b;
// }

// uint32_t    _feisel(uint32_t in, uint32_t s[4][256])
// {
//     uint8_t    s0_key;
//     uint8_t    s1_key;
//     uint8_t    s2_key;
//     uint8_t    s3_key;

//     s0_key = (in & 0xff000000) >> 24;
//     s1_key = (in & 0xff0000) >> 16 ;
//     s2_key = (in & 0xff00) >> 8;
//     s3_key = in & 0xff;

//     // fprintf(stderr, "%hhx|%hhx|%hhx|%hhx == %x %x %hhx %hx \n", s0_key, s1_key, s2_key, s3_key, in, 0x12345678, (char)0x12345678, (short)0x12345678 << 4);
//     // (void)s;
//     return ((s[0][s0_key] + s[1][s1_key]) ^ s[2][s2_key]) + s[3][s3_key];
// }

uint64_t    _blowfish_decrypt(uint64_t block, uint32_t p[18], uint32_t s[4][256])
{
    uint32_t    l;
    uint32_t    r;

    l = block >> 32;
    r = block & 0xffffffff;
    // l = block & 0x0123456789123456;
    
    for (int i = 17; i > 1; i--)
    {
        l = l ^ p[i];
        r = r ^ feisel(l, s);
        uint32_swap(&l, &r);
    }
    uint32_swap(&l, &r);
    l = l ^ p[0];
    r = r ^ p[1];
    
    // printf("\n%x | %x  %lx\n", l, r, block);
    return ((uint64_t)l << 32) | (uint64_t)r;
    return 0;
}

uint64_t    _blowfish_encrypt(uint64_t block, uint32_t p[18], uint32_t s[4][256])
{
    uint32_t    l;
    uint32_t    r;

    l = block >> 32;
    r = block & 0xffffffff;
    // l = block & 0x0123456789123456;
    
    // fprintf(stderr, "hallo\n");
    // printf("\n%x | %x  %lx %lx\n", l, r, block, ((uint64_t)l << 32) | (uint64_t)r);
    for (int i = 0; i < 16; i++)
    {
        l = l ^ p[i];
        r = r ^ feisel(l, s);
        uint32_swap(&l, &r);
    }
    uint32_swap(&l, &r);
    l = l ^ p[17];
    r = r ^ p[16];
    // printf("\n%x | %x  %lx %lx\n", l, r, block, ((uint64_t)l << 32) | (uint64_t)r);
    return ((uint64_t)l << 32) | (uint64_t)r;
}

void    blowfish_init(char *key)
{
    size_t key_len;
    uint32_t    p[18];
    uint32_t    s[4][256];
    uint32_t    ext_key[18];
    char        *bext_key;
    // unsigned char        *p_fill;
    // unsigned char        *s_fill;

    key_len = strlen(key);
    if (key_len < 4 || key_len > 56)
    {
        fprintf(stderr, "blowfish error: key length should lie between 4 and 56 bytes\n");
        exit(EXIT_FAILURE);
    }
    
    bext_key = (char *)ext_key;
    for (int i = 0; i < 72; i++)
        bext_key[i] = key[i % key_len];
    
    // p_fill = (unsigned char *)p;
    // s_fill = (unsigned char *)s;
    // int n = 0;
    // for (int i = 0; i < 18 * 4; i++)
    // {
    //     p_fill[i] = (bbp_getnth_term(n) << 4) | (bbp_getnth_term(n + 1));
    //     // fprintf(stderr, "%x", p_fill[i]);
    //     n += 2;
    // }
    // // for (int i = 0; i < 4; i++)
    // for (int i = 0; i < 256 * 4 * 4; i++)
    // {
    //     s_fill[i] = (bbp_getnth_term(n) << 4) | (bbp_getnth_term(n + 1));
    //     // fprintf(stderr, "%x, %d\n", s_fill[i], i);
    //     n += 2;
    // }
    
    for (int i = 0; i < 18; i++)
        p[i] = original_p[i] ;//^ ext_key[i];
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 256; j++)
            s[i][j] = original_s[i][j];
    

    uint64_t init_val = 0;
    
    for (int i = 0; i < 18; i += 2)
    {
        init_val = blowfish_encrypt(init_val, p, s);
        p[i] = init_val >> 32;
        p[i + 1] = init_val & 0xffffffff;
    }
    
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 256; j += 2)
        {
            init_val = blowfish_encrypt(init_val, p, s);
            s[i][j] = init_val >> 32;
            s[i][j + 1] = init_val & 0xffffffff;
        }


    uint64_t encr, encr_tr, decr, decr_tr;

    encr = blowfish_encrypt(0x4242424242424242, p, s);
    encr_tr = _blowfish_encrypt(0x4242424242424242, p, s);
    decr = blowfish_decrypt(encr_tr, p, s);
    decr_tr = _blowfish_decrypt(encr_tr, p, s);
    fprintf(stderr, "%lx==%lx %lx==%lx\n", encr, encr_tr, decr, decr_tr);
    for (int i = 0; i < 1000; i++)
        fprintf(stderr, "%x", bbp_getnth_term(i));
    // printf("\n%f %f\n", floor(1.2f), ceil(-1.2f));
    // printf("%f %f\n", floor(1.5f), ceil(-1.5f));
    // printf("%f %f\n", floor(1.8f), ceil(-1.8f));
    (void)p;
    (void)s;
}