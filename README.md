
# SignalSpyChecker

**SignalSpyChecker** is a lightweight C++ library for validating Qt signals in your unit tests. 
It provides two tools: `Constant` and `Optional`, for verifying signal counts and value ranges. 
The library is built on top of `QSignalSpy` and integrates seamlessly 
with [Catch2](https://github.com/catchorg/Catch2) for testing. SignalSpyChecker is particularly 
useful for monitoring multiple QSignalSpy instances simultaneously, 
ensuring that no unexpected extra signals are emitted during a single action.

## Features
- **SignalSpyChecker::Constant**: Validates that signals are emitted a specific, fixed number of times.
- **SignalSpyChecker::Optional**: Validates that signals are emitted within a range of possible counts.
- Built-in support for custom signal names via `SignalSpy`.
- Debugging tools to inspect signals and expected counts.

## Getting Started

### Prerequisites
- Qt 6
- C++17 or later
- [Catch2 v3 Testing Framework](https://github.com/catchorg/Catch2)

### Installation
1. Clone the repository or copy the `SpyChecker.h` and `SpyChecker.cpp` files into your project.
2. Include `SignalSpyChecker.h` in your tests.

## Usage

### Basic Example
Here’s how to use the `SignalSpyChecker` library in a unit test:

```cpp
#include <SignalSpyChecker.h>
#include <QObject>
#include <catch2/catch.hpp>

using namespace SignalSpyChecker;

class MyObject : public QObject {
    Q_OBJECT
public:
    void triggerSignal() { emit mySignal(); }
signals:
    void mySignal();
};

TEST_CASE("SignalSpyChecker Basic Example") {
    MyObject obj;

    // Create a Constant checker to expect 1 signal emission
    Constant checker = { { new SignalSpy(&obj, &MyObject::mySignal), 1 } };

    // Trigger the signal
    obj.triggerSignal();

    // Verify the signal count
    checker.checkSpies();
}
```

### Optional Example
```cpp
TEST_CASE("SignalSpyChecker Optional Example") {
    MyObject obj;

    // Create an Optional checker to allow 1 or 2 signal emissions
    Optional checker = { { new SignalSpy(&obj, &MyObject::mySignal), { 1, 2 } } };

    // Trigger the signal twice
    obj.triggerSignal();
    obj.triggerSignal();

    // Verify the signal count falls within the expected range
    checker.requireSpies();
}
```

### Using `makeChecker`
The `makeChecker` function in `Constant` automatically creates signal spies for all signals in an object:

```cpp
TEST_CASE("SignalSpyChecker makeChecker Example") {
    MyObject obj;

    // Automatically create spies for all signals
    Constant checker = Constant::makeChecker(&obj);

    // Trigger the signal
    obj.triggerSignal();

    // Verify the signal count
    checker.checkSpies();
}
```

## Classes

### `SignalSpyChecker::SignalSpy`
A subclass of `QSignalSpy` with additional functionality for assigning object names.

#### Methods:
- `SignalSpy(const QObject *obj, const char *aSignal)`: Constructor for a signal spy.
- `void setObjectName(const QString &name)`: Sets the name of the signal spy.
- `QString objectName() const`: Gets the name of the signal spy.

---

### `SignalSpyChecker::Constant`
Validates that signals are emitted a specific, fixed number of times.

#### Methods:
- `Constant()`: Default constructor.
- `Constant(const std::initializer_list<std::pair<SignalSpy*, int>> &list)`: Constructs a checker with expected signal counts.
- `void checkSpies() const`: Verifies the signal counts.
- `void requireSpies() const`: Enforces the signal counts (throws an error on failure).
- `void clearSpyCounts()`: Clears recorded signal counts.
- `static Constant makeChecker(QObject* object)`: Automatically creates signal spies for all signals in an object.

---

### `SignalSpyChecker::Optional`
Validates that signals are emitted within a range of possible counts.

#### Methods:
- `Optional()`: Default constructor.
- `Optional(const std::initializer_list<std::pair<SignalSpy*, QList<int>>> &list)`: Constructs a checker with possible expected counts.
- `void checkSpies() const`: Verifies that signal counts fall within the expected range.
- `void requireSpies() const`: Enforces that signal counts are within the range (throws an error on failure).
- `void clearSpyCounts()`: Clears recorded signal counts.

---

## Debugging
If a test fails, detailed information about the signal spies and expected counts will be output via `qDebug` and Catch2’s `INFO` macro. Place breakpoints as indicated in the debug output to investigate issues.

## Contributing
Contributions are welcome! Feel free to submit a pull request or open an issue.

## License
This library is licensed under the MIT License. See `LICENSE` for details.

## Acknowledgments
- Built on `QSignalSpy` from the Qt framework.
- Testing framework: [Catch2](https://github.com/catchorg/Catch2).

