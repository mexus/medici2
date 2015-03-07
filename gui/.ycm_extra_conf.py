import os
import ycm_core
from clang_helpers import PrepareClangFlags

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )

# Set this to the absolute path to the folder (NOT the file!) containing the
# compile_commands.json file to use that instead of 'flags'. See here for
# more details: http://clang.llvm.org/docs/JSONCompilationDatabase.html
# Most projects will NOT need to set this to anything; you can just change the
# 'flags' list of compilation flags. Notice that YCM itself uses that approach.
compilation_database_folder = os.path.join(DirectoryOfThisScript(), '../build/')

# These are the compilation flags that will be used in case there's no
# compilation database set.
flags = [
'-std=c++11',
'-x', 'c++',
'-I', '../source/',
'-I', '/usr/include/c++/4.8/',

'-DQT_CORE_LIB',
'-DQT_GUI_LIB',
'-DQT_NETWORK_LIB',
'-DQT_QML_LIB',
'-DQT_QUICK_LIB',
'-DQT_SQL_LIB',
'-DQT_WIDGETS_LIB',
'-DQT_XML_LIB'

]

def FindQt5Location():
    prefixes = ["/usr/include/qt5", "/usr/include/qt"]
    for prefix in prefixes:
        if os.path.isdir(prefix + '/QtCore'):
            return prefix
    raise Exception('Not found qt')

def FindQtComponents(prefix = "/usr/include/"):
    if FindQtComponents.has_run:
        return
    FindQtComponents.has_run = True
    location = FindQt5Location()
    components = [ d for d in os.listdir(location) if os.path.isdir(os.path.join(location, d)) ]
    flags.extend(['-I', location])
    for component in components:
        inclusion_path = os.path.join(location, component)
        flags.extend(['-I', inclusion_path])
FindQtComponents.has_run = False

if compilation_database_folder:
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return flags
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def FlagsForFile( filename ):
  FindQtComponents()
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = database.GetCompilationInfoForFile( filename )
    final_flags = PrepareClangFlags(
        MakeRelativePathsInFlagsAbsolute(
            compilation_info.compiler_flags_,
            compilation_info.compiler_working_dir_ ),
        filename )
  if not final_flags:
    relative_to = DirectoryOfThisScript()
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }
i = 0

