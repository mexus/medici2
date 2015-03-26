import os
import ycm_core
from clang_helpers import PrepareClangFlags


class Flags:
    def __init__(self, include_tests=True, include_qt=True):
        self.flags = [
            '-std=c++11',
            '-x', 'c++',
            '-I', 'source/',
        ]
        self.include_qt = include_qt
        self.qt_components_found = False
        self.compilation_database_folder = os.path.join(self.directory_of_this_script(), '../build/')
        self.database = None
        if include_qt:
            self.flags.extend([
                '-DQT_CORE_LIB',
                '-DQT_GUI_LIB',
                '-DQT_NETWORK_LIB',
                '-DQT_QML_LIB',
                '-DQT_QUICK_LIB',
                '-DQT_SQL_LIB',
                '-DQT_WIDGETS_LIB',
                '-DQT_XML_LIB'
            ])
        if include_tests:
            self.flags.extend(['-I', 'build/test/'])

    @staticmethod
    def directory_of_this_script():
        return os.path.dirname(os.path.abspath(__file__))

    @staticmethod
    def find_qt5_location():
        prefixes = ["/usr/include/qt5", "/usr/include/qt"]
        for prefix in prefixes:
            if os.path.isdir(prefix + '/QtCore'):
                return prefix
        raise Exception('Not found qt')

    def find_qt_components(self):
        if self.qt_components_found:
            return
        self.qt_components_found = True
        location = self.find_qt5_location()
        components = [d for d in os.listdir(location) if os.path.isdir(os.path.join(location, d))]
        self.flags.extend(['-I', location])
        for component in components:
            inclusion_path = os.path.join(location, component)
            self.flags.extend(['-I', inclusion_path])

        if self.compilation_database_folder:
            self.database = ycm_core.CompilationDatabase(self.compilation_database_folder)
        else:
            self.database = None

    @staticmethod
    def make_relative_paths_in_flags_absolute(flags, working_directory):
        if not working_directory:
            return flags
        new_flags = []
        make_next_absolute = False
        path_flags = ['-isystem', '-I', '-iquote', '--sysroot=']
        for flag in flags:
            new_flag = flag
            if make_next_absolute:
                make_next_absolute = False
                if not flag.startswith('/'):
                    new_flag = os.path.join(working_directory, flag)
            for path_flag in path_flags:
                if flag == path_flag:
                    make_next_absolute = False
                    break
                if flag.startswith(path_flag):
                    path = flag[len(path_flag):]
                    new_flag = path_flag + os.path.join(working_directory, path)
            if new_flag:
                new_flags.append(new_flag)
        return new_flags

    def flags_for_file(self, filename):
        self.find_qt_components()
        final_flags = None
        if self.database:
            # Bear in mind that compilation_info.compiler_flags_ does NOT return a
            # python list, but a "list-like" StringVec object
            compilation_info = self.database.GetCompilationInfoForFile(filename)
            final_flags = PrepareClangFlags(
                self.make_relative_paths_in_flags_absolute(
                    compilation_info.compiler_flags_,
                    compilation_info.compiler_working_dir_),
                filename)
        if not final_flags:
            relative_to = self.directory_of_this_script()
            final_flags = self.make_relative_paths_in_flags_absolute(self.flags, relative_to)
        return {
            'flags': final_flags,
            'do_cache': True
        }

