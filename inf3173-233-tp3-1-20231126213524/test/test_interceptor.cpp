#include <gtest/gtest.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <istream>
#include <iterator>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "inf3173/config.h"
#include "interceptor.h"
#include "testutils.h"
#include "utils.h"
#include "wrapper.h"

void clean(const char* fname) {
    if (is_regular_file(fname)) {
        unlink(fname);
    }
}

static const char* slomo_exe = BINARY_DIR "/bin/slomo";
static const char* inter_exe = BINARY_DIR "/bin/interceptor";

TEST(Interceptor, Ipsum) {
    const char* datafile = SOURCE_DIR "/data/ipsum.txt";
    const char* logfile = BINARY_DIR "/ipsum_logfile.bin";
    const char* infile = BINARY_DIR "/ipsum_input.bin";
    const char* outfile = BINARY_DIR "/ipsum_output.bin";

    clean(logfile);
    clean(infile);
    clean(outfile);

    std::stringstream ss;
    ss << inter_exe << " --stdin " << infile << " --stdout " << outfile << " "
       << "hexdump < " << datafile << " > " << logfile << " 2>/dev/null";
    std::string cmd = ss.str();

    int ret = std::system(cmd.c_str());

    ASSERT_EQ(ret, 0) << "Code de retour" << std::endl
                      << "commande: " << cmd << std::endl;

    ASSERT_TRUE(are_files_identical(datafile, infile))
        << "input files error\n ref:" << datafile << "\n act:" << infile;
    ASSERT_TRUE(are_files_identical(logfile, outfile))
        << "output files error\n ref:" << logfile << "\n act:" << outfile;
}

TEST(Interceptor, Slomo) {
    const char* datafile = SOURCE_DIR "/data/cat.jpg";
    const char* logfile = BINARY_DIR "/slomo_logfile.bin";
    const char* infile = BINARY_DIR "/slomo_input.bin";
    const char* outfile = BINARY_DIR "/slomo_output.bin";

    clean(logfile);
    clean(infile);
    clean(outfile);

    std::stringstream ss;
    ss << inter_exe << " --stdin " << infile << " --stdout " << outfile << " "
       << slomo_exe << " < " << datafile << " > " << logfile << " 2>/dev/null";
    std::string cmd = ss.str();

    int ret = std::system(cmd.c_str());

    ASSERT_EQ(ret, 0) << "Code de retour" << std::endl
                      << "commande: " << cmd << std::endl;

    ASSERT_TRUE(are_files_identical(datafile, infile))
        << "input files error\n ref:" << datafile << "\n act:" << infile;
    ASSERT_TRUE(are_files_identical(logfile, outfile))
        << "output files error\n ref:" << logfile << "\n act:" << outfile;
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
