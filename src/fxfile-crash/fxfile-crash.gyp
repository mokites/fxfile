#
# Copyright (c) 2013 Leon Lee author. All rights reserved.
#
#   homepage: http://www.flychk.com
#   e-mail:   mailto:flychk@flychk.com
#
# Use of this source code is governed by a GPLv3 license that can be
# found in the LICENSE file.

{
    'variables':
    {
    },
    
    'includes':
    [
        '../../build/common.gypi',
    ],
    
    'target_defaults':
    {
        'default_configuration' : 'Debug-x86-Unicode',

        'configurations':
        {
            'Debug-x86-Unicode':
            {
                'inherit_from': ['Debug-x86-Unicode_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-unicode',
                    'IntermediateDirectory': '../../obj/fxfile-crash/dbg-x86-unicode',
                    'TargetName':            '$(ProjectName)_dbg',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName)_dbg.dll',
                        'AdditionalLibraryDirectories':
                        [
                        ],
                        'AdditionalDependencies':
                        [
                        ],
                        'conditions':
                        [
                            [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                            [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                        ],
                    },
                },
            },
            
            'Release-x86-Unicode':
            {
                'inherit_from': ['Release-x86-Unicode_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-unicode',
                    'IntermediateDirectory': '../../obj/fxfile-crash/rel-x86-unicode',
                    'TargetName':            '$(ProjectName)',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName).dll',
                        'AdditionalLibraryDirectories':
                        [
                        ],
                        'AdditionalDependencies':
                        [
                        ],
                        'conditions':
                        [
                            [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                            [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                        ],
                    },
                },
            },
            
            'Debug-x86-Multibyte':
            {
                'inherit_from': ['Debug-x86-Multibyte_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-multibyte',
                    'IntermediateDirectory': '../../obj/fxfile-crash/dbg-x86-multibyte',
                    'TargetName':            '$(ProjectName)_dbg',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName)_dbg.dll',
                        'AdditionalLibraryDirectories':
                        [
                        ],
                        'AdditionalDependencies':
                        [
                        ],
                        'conditions':
                        [
                            [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                            [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                        ],
                    },
                },
            },
            
            'Release-x86-Multibyte':
            {
                'inherit_from': ['Release-x86-Multibyte_Base'],
                
                'msvs_configuration_attributes':
                {
                    'OutputDirectory':       '../../bin/x86-multibyte',
                    'IntermediateDirectory': '../../obj/fxfile-crash/rel-x86-multibyte',
                    'TargetName':            '$(ProjectName)',
                },
                
                'msvs_settings':
                {
                    'VCLinkerTool': 
                    {
                        'OutputFile': '$(OutDir)\$(ProjectName).dll',
                        'AdditionalLibraryDirectories':
                        [
                        ],
                        'AdditionalDependencies':
                        [
                        ],
                        'conditions':
                        [
                            [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                            [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                        ],
                    },
                },
            },

            'conditions':
            [
                [ 'target_arch!="x86"',
                    {
                        'Debug-x64-Unicode':
                        {
                            'inherit_from': ['Debug-x64-Unicode_Base'],
                            
                            'msvs_configuration_attributes':
                            {
                                'OutputDirectory':       '../../bin/x64',
                                'IntermediateDirectory': '../../obj/fxfile-crash/dbg-x64',
                                'TargetName':            '$(ProjectName)_dbg',
                            },
                            
                            'msvs_settings':
                            {
                                'VCLinkerTool': 
                                {
                                    'OutputFile': '$(OutDir)\$(ProjectName)_dbg.dll',
                                    'AdditionalLibraryDirectories':
                                    [
                                    ],
                                    'AdditionalDependencies':
                                    [
                                    ],
                                    'conditions':
                                    [
                                        [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                                        [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                                    ],
                                },
                            },
                        },

                        'Release-x64-Unicode':
                        {
                            'inherit_from': ['Release-x64-Unicode_Base'],
                            
                            'msvs_configuration_attributes':
                            {
                                'OutputDirectory':       '../../bin/x64',
                                'IntermediateDirectory': '../../obj/fxfile-crash/rel-x64',
                                'TargetName':            '$(ProjectName)',
                            },
                            
                            'msvs_settings':
                            {
                                'VCLinkerTool': 
                                {
                                    'OutputFile': '$(OutDir)\$(ProjectName).dll',
                                    'AdditionalLibraryDirectories':
                                    [
                                    ],
                                    'AdditionalDependencies':
                                    [
                                    ],
                                    'conditions':
                                    [
                                        [ 'msvs_version == 2008', { 'AdditionalDependencies' : [ '$(INHERIT)', ], }, ],
                                        [ 'msvs_version != 2008', { 'AdditionalDependencies' : [ '%(AdditionalDependencies)', ], }, ],
                                    ],
                                },
                            },
                        },
                    },
                ],
            ],
        },
    },

    'targets':
    [
        {
            'target_name': 'fxfile-crash',
            
            'type': 'shared_library',
            
            'defines':
            [
                'FXFILE_CRASH_DL_EXPORTS',
            ],
            
            'include_dirs':
            [
                './',
                '../../lib/zlib/include',
            ],
            
            'libraries':
            [
                'ws2_32.lib',
                'comctl32.lib',
                'version.lib',
                'wininet.lib',
                'shlwapi.lib',
            ],
            
            'msvs_settings':
            {
                'VCLinkerTool': 
                {
                    'AdditionalLibraryDirectories':
                    [
                        '$(OutDir)',
                        '../../lib/zlib/lib',
                    ]
                },
            },
            
            'msvs_precompiled_header': 'stdafx.h',
            'msvs_precompiled_source': 'stdafx.cpp',
            
            'sources':
            [
                './AboutDlg.cpp',
                './SendMailDlg.cpp',
                './SimpleDlg.cpp',
                './AboutDlg.h',
                './SendMailDlg.h',
                './SimpleDlg.h',
                './ColHelper.cpp',
                './StrHolder.cpp',
                './Array.h',
                './ColHelper.h',
                './Hash.h',
                './InMemLogFile.cpp',
                './InMemLogFile.h',
                './LogStream.cpp',
                './LogStream.h',
                './ModuleImportTable.cpp',
                './ModuleImportTable.h',

                './TransferProgressDlg.cpp',
                './TransferProgressDlg.h',
                './DescribeErrorDlg.cpp',
                './DescribeErrorDlg.h',
                './WaitDlg.cpp',
                './WaitDlg.h',
                './MainDlg.cpp',
                './MainDlg.h',

                './PreviewDlg.cpp',
                './PreviewDlg.h',
                './MachineInfoDlg.cpp',
                './MachineInfoDlg.h',
                './MachineStateDlg.cpp',
                './MachineStateDlg.h',
				
                './InPlaceNew.h',
                './InterfacePtr.h',
                './List.h',
                './SmartPtr.h',
                './StrHolder.h',
                './AnimProgressBar.cpp',
                './HexView.cpp',
                './HyperLink.cpp',
                './ImageView.cpp',
                './LayoutManager.cpp',
                './Splitter.cpp',
                './TextView.cpp',
                './WaitCursor.cpp',
                './AnimProgressBar.h',
                './HexView.h',
                './HyperLink.h',
                './ImageView.h',
                './LayoutManager.h',
                './Splitter.h',
                './TextView.h',
                './WaitCursor.h',
                './CMapi.cpp',
                './EnumProcess.cpp',
                './LogFile.cpp',
                './NetThunks.cpp',
                './SymEngine.cpp',
                './SymEngineNet.cpp',
                './TextLogFile.cpp',
                './ThemeXP.cpp',
                './XmlLogFile.cpp',
                './CMapi.h',
                './EnumProcess.h',
                './LogFile.h',
                './LogLink.h',
                './NetThunks.h',
                './SymEngine.h',
                './SymEngineNet.h',
                './TextLogFile.h',
                './ThemeXP.h',
                './XmlLogFile.h',
                './BugTrap.cpp',
                './BugTrapNet.cpp',
                './BugTrapUI.cpp',
                './BugTrapUtils.cpp',
                './ResManager.cpp',
                './BTAtlWindow.h',
                './BTMfcWindow.h',
                './BTTrace.h',
                './BugTrap.h',
                './BugTrapNet.h',
                './BugTrapUI.h',
                './BugTrapUtils.h',
                './fxfile-crash.h',
                './ResManager.h',
                './stdafx.cpp',
                './stdafx.h',
                './Globals.cpp',
                './Globals.h',
                './LeakWatcher.h',
                './resource.h',
                './VersionInfo.h',
                './AssemblyInfo.cpp',
                './BugTrap.def',
                './BugTrap.rc',
                './Encoding.cpp',
                './TextFormat.cpp',
                './Encoding.h',
                './TextFormat.h',
                './XmlReader.cpp',
                './XmlWriter.cpp',
                './XmlReader.h',
                './XmlWriter.h',
                './FileStream.cpp',
                './InputStream.cpp',
                './MemStream.cpp',
                './OutputStream.cpp',
                './StrStream.cpp',
                './BaseStream.h',
                './FileStream.h',
                './InputStream.h',
                './MemStream.h',
                './OutputStream.h',
                './Stream.h',
                './StrStream.h',

                './res/Bug.ico',
            ],
        },
    ],
}