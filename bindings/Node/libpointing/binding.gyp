{
  "targets": [
	{
	  "target_name": "pointing",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
	  "sources": [
		"pointing/pointing.cc",
		"pointing/npointingdevice.cc",
		"pointing/ndisplaydevice.cc",
		"pointing/ntransferfunction.cc",
		"pointing/npointingdevicemanager.cc",
		"pointing/ndisplaydevicemanager.cc",
		"pointing/nsystempointeracceleration.cc",
	  ],
	  'conditions': [
			['OS=="mac"', {
				"link_settings": {
					'libraries': [
					   '-lpointing',
					   '-L/usr/local/lib'
					 ]
				 },
				"include_dirs": [
					'/usr/local/include',
					"<!@(node -p \"require('node-addon-api').include\")"
				],
				'xcode_settings': {
					'MACOSX_DEPLOYMENT_TARGET': '10.7',

					'OTHER_CFLAGS': [
					  "-stdlib=libc++"
					],
				},
			}],
			['OS=="linux"', {
				"link_settings": {
				'libraries': [
				   '/usr/lib/pointing.so'
				 ]},
				"include_dirs": [
                   '-lpointing',
				   '-L/usr/local/lib',
					"<!@(node -p \"require('node-addon-api').include\")"
				]
			}],
			['OS=="win"', {
				"link_settings": {
					"conditions": [
						["target_arch == 'ia32'", {
							'libraries': [
								'-lsetupapi',
								'-lhid',
								'-luser32',
								'-ladvapi32',
                                '../../../../pointing/Release/pointing.lib'
							]
						}],
						["target_arch == 'x64'", {
							'libraries': [
								'-lsetupapi',
								'-lhid',
								'-luser32',
								'-ladvapi32',
								'../../../../pointing/x64/Release/pointing.lib'
							]
						}]
					]
				 },
				"include_dirs": [
					'../../..',
                    "<!@(node -p \"require('node-addon-api').include\")"
				],
				'configurations': {
					'Debug': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'RuntimeLibrary': '3' # /MDd
							},
						},
					},
					'Release': {
						'msvs_settings': {
							'VCCLCompilerTool': {
								'RuntimeLibrary': '2' # /MD
							},
						},
					},
				},
				"msvs_disabled_warnings": [ 4244, 4267 ],
			}],
		],
      'dependencies': [
          "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
	}
  ]
}
