#include <iostream>

namespace uart {
namespace test {

extern int tests_run;
extern int tests_passed;
extern int tests_failed;

extern int runBasicTests();
extern int runFifoTests();

} // namespace test
} // namespace uart

int main() {
    std::cout << "UART Driver Test Suite" << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // Run all test suites
    uart::test::runBasicTests();
    uart::test::runFifoTests();
    
    // Print summary
    std::cout << "\n=======================================" << std::endl;
    std::cout << "Test Summary" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Total tests:  " << uart::test::tests_run << std::endl;
    std::cout << "Passed:       " << uart::test::tests_passed << std::endl;
    std::cout << "Failed:       " << uart::test::tests_failed << std::endl;
    std::cout << "=======================================" << std::endl;
    
    if (uart::test::tests_failed > 0) {
        std::cout << "\nSome tests FAILED! Please investigate." << std::endl;
        return 1;
    } else {
        std::cout << "\nAll tests PASSED!" << std::endl;
        return 0;
    }
}
