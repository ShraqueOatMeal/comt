#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// Bluetooth device class to simulate Bluetooth functionality
class BluetoothDevice {
public:
  string name;
  string mac;
  string type;
  bool paired;

  BluetoothDevice(string n, string m, string t)
      : name(n), mac(m), type(t), paired(false) {}
};

class Radio {
private:
  bool power;
  double frequency;
  int volume;
  string mode; // "FM", or "BT" (Bluetooth)
  vector<double> presets;
  vector<BluetoothDevice> bluetoothDevices;
  int connectedDeviceIndex;

  // Station simulation data
  struct Station {
    double frequency;
    string name;
    string genre;
  };
  vector<Station> stations;

public:
  Radio()
      : power(false), frequency(88.1), volume(10), mode("FM"),
        connectedDeviceIndex(-1) {
    // Initialize with some preset stations
    presets = {88.1, 92.3, 95.5};

    // Initialize simulated stations
    stations = {{88.1, "ROCK FM", "Rock"},
                {92.3, "HITS FM", "Pop"},
                {95.5, "JAZZ ST", "Jazz"}};

    // Initialize with some potential Bluetooth devices in range
    bluetoothDevices = {
        {"iPhone 13", "A4:83:E7:12:9B:3D", "Phone"},
        {"Bose SoundLink", "00:0C:8A:E1:57:32", "Speaker"},
        {"Sony WH-1000XM4", "38:18:4C:7D:5A:E9", "Headphones"},
        {"Samsung Galaxy", "B4:7C:9C:8E:11:5F", "Phone"},
        {"Car Audio System", "00:1D:43:A0:7B:F2", "Car Stereo"}};

    srand(time(nullptr));
  }

  bool getPower() { return this->power; }

  void togglePower() {
    power = !power;
    if (power) {
      cout << "Radio powered ON\n";
      displayInfo();
    } else {
      cout << "Radio powered OFF\n";
    }
  }

  void volumeUp() {
    if (!power || (mode == "BT" && connectedDeviceIndex < 0))
      return;

    if (volume < 30) {
      volume++;
      cout << "Volume: " << volume << "/30\n";
    }
  }

  void volumeDown() {
    if (!power || (mode == "BT" && connectedDeviceIndex < 0))
      return;

    if (volume > 0) {
      volume--;
      cout << "Volume: " << volume << "/30\n";
    }
  }

  void switchMode() {
    if (!power || connectedDeviceIndex >= 0) {
      cout << "Please disconnect from a Bluetooth device first\n";
      return;
    }

    if (mode == "FM") {
      mode = "BT";
      cout << "Switched to Bluetooth mode\n";
      if (connectedDeviceIndex >= 0) {
        cout << "Connected to: " << bluetoothDevices[connectedDeviceIndex].name
             << "\n";
      } else {
        cout << "No Bluetooth device connected. Use 'b' to scan for devices.\n";
      }
    } else { // BT
      mode = "FM";
      frequency = 88.1; // Default FM frequency
      cout << "Switched to FM mode\n";
    }

    displayInfo();
  }

  void loadPreset(int position) {
    if (!power || position < 1 || position > 5 || mode == "BT")
      return;

    frequency = presets[position - 1];
    cout << "Loaded preset " << position << ": ";
    displayInfo();
  }

  void scanBluetooth() {
    if (!power || mode != "BT") {
      cout << "Please switch to Bluetooth mode first\n";
      return;
    }

    cout << "Scanning for Bluetooth devices...\n";

    // Simulate Bluetooth scanning
    for (int i = 0; i < 5; i++) {
      cout << "Searching";
      for (int j = 0; j < 3; j++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(300));
      }
      cout << "\n";
    }

    // Randomize available devices
    int availableDevices = 2 + (rand() % (bluetoothDevices.size() - 1));

    cout << "\nFound " << availableDevices << " Bluetooth devices:\n";
    cout << "-------------------------------------\n";
    cout << "ID | Device Name          | Type      | Status\n";
    cout << "-------------------------------------\n";

    for (int i = 0; i < availableDevices; i++) {
      cout << setw(2) << (i + 1) << " | ";
      cout << setw(20) << left << bluetoothDevices[i].name << " | ";
      cout << setw(9) << left << bluetoothDevices[i].type << " | ";

      if (i == connectedDeviceIndex) {
        cout << "Connected";
      } else if (bluetoothDevices[i].paired) {
        cout << "Paired";
      } else {
        cout << "Available";
      }
      cout << "\n";
    }

    cout << "-------------------------------------\n";
    cout << "To connect to a device, enter 'c' followed by the device ID "
            "(e.g., c1)\n";

    // Switch to Bluetooth mode
    if (mode != "BT") {
      mode = "BT";
      cout << "Switched to Bluetooth mode\n";
    }
  }

  void connectBluetoothDevice(int deviceId) {
    if (!power || mode != "BT") {
      cout << "Please switch to Bluetooth mode first\n";
      return;
    }

    deviceId--; // Convert from 1-based to 0-based index

    if (deviceId >= 0 && deviceId < static_cast<int>(bluetoothDevices.size())) {
      // Simulate connection process
      cout << "Connecting to " << bluetoothDevices[deviceId].name << "...\n";

      for (int i = 0; i < 3; i++) {
        cout << "Establishing connection";
        for (int j = 0; j < 3; j++) {
          cout << ".";
          cout.flush();
          this_thread::sleep_for(chrono::milliseconds(300));
        }
        cout << "\n";
      }

      connectedDeviceIndex = deviceId;
      bluetoothDevices[deviceId].paired = true;
      cout << "Successfully connected to " << bluetoothDevices[deviceId].name
           << "\n";
    } else {
      cout << "Invalid device ID\n";
    }

    displayInfo();
  }

  void disconnectBluetooth() {
    if (!power || mode != "BT" || connectedDeviceIndex < 0) {
      cout << "No Bluetooth device currently connected\n";
      return;
    }

    cout << "Disconnecting from " << bluetoothDevices[connectedDeviceIndex].name
         << "...\n";
    this_thread::sleep_for(chrono::milliseconds(800));
    cout << "Device disconnected\n";

    connectedDeviceIndex = -1;
    displayInfo();
  }

  void displayInfo() {
    if (!power)
      return;

    cout << "┌────────────────────────────────────┐\n";
    cout << "│ ";

    if (mode == "FM") {
      if (mode == "FM") {
        cout << fixed << setprecision(1) << frequency << " " << mode;
      }
    } else { // Bluetooth
      cout << "Bluetooth";
      if (connectedDeviceIndex >= 0) {
        cout << " - " << bluetoothDevices[connectedDeviceIndex].name;
      } else {
        cout << " - Not Connected";
      }
    }

    cout << string(33 - ((mode == "BT")
                             ? 9 + (connectedDeviceIndex >= 0
                                        ? bluetoothDevices[connectedDeviceIndex]
                                                  .name.length() +
                                              3
                                        : 14)
                             : (mode.length() + 6)),
                   ' ')
         << "│\n";

    // Display station info if on a known frequency and in radio mode
    if (mode != "BT") {
      bool knownStation = false;
      for (const auto &station : stations) {
        if (abs(station.frequency - frequency) < 0.1) {
          cout << "│ " << station.name << " - " << station.genre;
          cout << string(
                      33 - (station.name.length() + station.genre.length() + 3),
                      ' ')
               << "│\n";
          knownStation = true;
          break;
        }
      }

      if (!knownStation) {
        cout << "│ No station information              │\n";
      }
    } else {
      // Bluetooth specific info
      if (connectedDeviceIndex >= 0) {
        cout << "│ " << bluetoothDevices[connectedDeviceIndex].type << " - "
             << bluetoothDevices[connectedDeviceIndex].mac;
        cout << string(
                    33 - (bluetoothDevices[connectedDeviceIndex].type.length() +
                          bluetoothDevices[connectedDeviceIndex].mac.length() +
                          3),
                    ' ')
             << "│\n";
      } else {
        cout << "│ No device connected                 │\n";
      }
    }

    // Volume display
    cout << "│ Volume: ";
    cout << volume << "/30";
    cout << string(25 - to_string(volume).length(), ' ') << "│\n";
    cout << "└────────────────────────────────────┘\n";

    // Show "now playing" simulation
    if (mode != "BT") {
      bool knownStation = false;
      for (const auto &station : stations) {
        if (abs(station.frequency - frequency) < 0.1) {
          simulatePlayback(station.genre);
          knownStation = true;
          break;
        }
      }

      if (!knownStation) {
        cout << "▶ *static noises*\n";
      }
    } else if (connectedDeviceIndex >= 0) {
      simulateBluetoothPlayback();
    }
  }

  void simulatePlayback(const string &genre) {
    // Generate a random "song" based on genre
    string artist, song;

    if (genre == "Rock") {
      vector<string> artists = {"AC/DC", "Led Zeppelin", "Queen", "Metallica"};
      vector<string> songs = {"Highway to Hell", "Stairway to Heaven",
                              "Bohemian Rhapsody", "Enter Sandman"};
      artist = artists[rand() % artists.size()];
      song = songs[rand() % songs.size()];
    } else if (genre == "Pop") {
      vector<string> artists = {"Taylor Swift", "Ed Sheeran", "Ariana Grande",
                                "Justin Bieber"};
      vector<string> songs = {"Blank Space", "Shape of You", "Thank U Next",
                              "Sorry"};
      artist = artists[rand() % artists.size()];
      song = songs[rand() % songs.size()];
    } else if (genre == "Classical") {
      vector<string> artists = {"Mozart", "Beethoven", "Bach", "Chopin"};
      vector<string> songs = {"Symphony No. 40", "Moonlight Sonata",
                              "Air on G String", "Nocturne Op. 9"};
      artist = artists[rand() % artists.size()];
      song = songs[rand() % songs.size()];
    } else if (genre == "Jazz") {
      vector<string> artists = {"Miles Davis", "John Coltrane",
                                "Duke Ellington", "Louis Armstrong"};
      vector<string> songs = {"So What", "Giant Steps", "Take the A Train",
                              "What a Wonderful World"};
      artist = artists[rand() % artists.size()];
      song = songs[rand() % songs.size()];
    } else if (genre == "News") {
      cout << "▶ Now broadcasting: Latest News and Updates\n";
      return;
    } else {
      vector<string> artists = {"Various Artist", "Unknown Artist", "Radio DJ",
                                "Guest Artist"};
      vector<string> songs = {"Unknown Track", "Station ID", "Featured Song",
                              "Popular Hit"};
      artist = artists[rand() % artists.size()];
      song = songs[rand() % songs.size()];
    }

    cout << "▶ Now playing: " << song << " by " << artist << "\n";
  }

  void simulateBluetoothPlayback() {
    // Different sets of songs based on the type of device
    string deviceType = bluetoothDevices[connectedDeviceIndex].type;

    vector<string> artists;
    vector<string> songs;

    if (deviceType == "Phone") {
      artists = {"Spotify", "Apple Music", "Amazon Music", "YouTube Music"};
      songs = {"Top Hits Playlist", "Your Favorites", "Discover Weekly",
               "Personal Mix"};
    } else if (deviceType == "Speaker" || deviceType == "Headphones") {
      artists = {"Bluetooth Audio", "Wireless Sound", "Connected Device",
                 "Audio Stream"};
      songs = {"Music Playback", "Audio Stream", "Connected Audio",
               "Wireless Playback"};
    } else if (deviceType == "Car Stereo") {
      artists = {"Car Audio", "Vehicle System", "Auto Player", "Drive Mix"};
      songs = {"Road Trip Mix", "Driving Playlist", "Commute Tunes",
               "Travel Music"};
    } else {
      artists = {"Unknown Device", "Bluetooth Source", "External Audio",
                 "Connected Source"};
      songs = {"Audio Stream", "Unknown Track", "Bluetooth Audio",
               "Wireless Sound"};
    }

    string artist = artists[rand() % artists.size()];
    string song = songs[rand() % songs.size()];

    cout << "▶ Bluetooth playing: " << song << " from " << artist << "\n";
  }
};

// Function to simulate terminal input handling
string getInput() {
  // Get input that might be a command with a number
  string input;
  cin >> input;
  return input;
}

int main() {
  Radio radio;
  bool running = true;
  string input;

  cout << "Terminal Radio Simulator\n";

  while (running) {
    cout << "\nEnter command: ";
    input = getInput();

    if (input.length() == 1 && input != "q") {
      char cmd = input[0];
      switch (cmd) {
      case 'p': // Power
        radio.togglePower();
        break;
      case '+': // Volume up
        radio.volumeUp();
        break;
      case '-': // Volume down
        radio.volumeDown();
        break;
      case 'a': // FM/Bluetooth toggle
        radio.switchMode();
        break;
      case 'b': // Scan Bluetooth
        radio.scanBluetooth();
        break;
      case 'x': // Disconnect Bluetooth
        radio.disconnectBluetooth();
        break;
      case '1':
      case '2':
      case '3':
        radio.loadPreset(cmd - '0');
        break;
      default:
        cout << "Unknown command.\n";
      }
    } else if (input.length() >= 2 && input[0] == 'c') {
      // Connect to Bluetooth device command
      try {
        int deviceId = stoi(input.substr(1));
        radio.connectBluetoothDevice(deviceId);
      } catch (exception &e) {
        cout << "Invalid device ID. Use format 'c#' where # is the device "
                "number.\n";
      }
    } else if (input == "q" && !radio.getPower()) {
      cout << "Exiting radio simulator. Goodbye!\n";
      break;
    } else {
      cout << "Unknown command.\n";
    }
  }

  return 0;
}
