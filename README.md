# DoorstepDropoffSDK Documentation

---

## iOS Integration

### Prerequisites

- **Development Environment:** Xcode (version 12 or later is recommended) with a target of iOS 15 or higher.
- **API Key:** A valid API key provided by DoorstepAI.

### Step 1: Add the SDK Dependency

Integrate the SDK using Swift Package Manager and import:

```swift
import DoorstepDropoffSDK
```

### Step 2: Configure Info.plist Permissions

Update your app's Info.plist file to include the necessary permissions:

```xml
<!-- Location Usage Descriptions -->
<key>NSLocationWhenInUseUsageDescription</key>
<string>This app requires access to your location for enhanced delivery inteligence</string>
<key>NSLocationAlwaysAndWhenInUseUsage</key>
<string>This app requires access to your location for enhanced delivery inteligence</string>

<!-- Optional: Motion & Sensor Data (if applicable) -->
<key>NSMotionUsageDescription</key>
<string>This app requires access to motion for enhanced delivery inteligence</string>

<!-- Background Location Mode -->
<key>UIBackgroundModes</key>
<array>
    <string>location</string>
</array>
```

### Step 3: Initialize the SDK

Set the API Key in your app entry point (e.g., `App` initializer or `AppDelegate`). `initCollectors` and `shouldGetConfig` default to `true`; override only if you need to defer initialization.

```swift
import DoorstepDropoffSDK

struct MyApp: App {
    init() {
        DoorstepAI.setApiKey(key: "YOUR_API_KEY")

    }
    
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
```

Root Component (optional, legacy support)

> Earlier versions required mounting `DoorstepAIRoot`. In the latest SDK this is no longer required; data collection starts with `startDelivery*`. `DoorstepAIRoot` remains supported for backwards compatibility and will continue to work for the foreseeable future.

```swift

struct ContentView: View {
    var body: some View {
        DoorstepAIRoot()
        // ...
    }
}
```

### Step 4: Starting and Stopping Delivery Tracking

Begin tracking with the async entrypoints. Each start method accepts an optional `timeoutSeconds` to automatically stop tracking after the provided duration. This serves as a backstop to account for any edge cases affecting the exit-geofence. Use this feature carefully, as an unoptimized configuration may affect the tracking output.

```swift
Task {
    // Start delivery using a Google PlaceID
    try await DoorstepAI.startDeliveryByPlaceID(
        placeID: "placeID_here",
        deliveryId: "some_delivery_id",
        timeoutSeconds: 1200 // optional
    )
    
    // Start delivery using a Google PlusCode
    try await DoorstepAI.startDeliveryByPlusCode(
        plusCode: "plusCode_here",
        deliveryId: "some_delivery_id"
    )
    
    // Start delivery using Address components
    let address = AddressType(
        streetNumber: "123",
        route: "Main St",
        subPremise: "Unit A",
        locality: "City",
        administrativeAreaLevel1: "State",
        postalCode: "12345"
    )
    try await DoorstepAI.startDeliveryByAddressType(
        address: address,
        deliveryId: "some_delivery_id"
    )
    
    // Start delivery using a single address string
    let addressString = "123 Main St, Unit A, City, State, 12345"
    try await DoorstepAI.startDeliveryByAddressString(
        address: addressString,
        deliveryId: "some_delivery_id"
    )
    
    // Start delivery using latitude/longitude and sub-unit
    try await DoorstepAI.startDeliveryByLatLng(
        latitude: 37.776,
        longitude: -122.417,
        subUnit: "Unit A",
        deliveryId: "some_delivery_id"
    )
}
```

Record a dropoff with the following event recording methods:

```swift
Task {
    // For a POD dropoff
    try await DoorstepAI.markDropoff(
        deliveryId: "some_delivery_id",
        dropoffType: .pod
    )

    // For a non-pod dropoff
    try await DoorstepAI.markDropoff(
        deliveryId: "some_delivery_id",
        dropoffType: .non_pod
    )
    
    // NOTE: The previous methods of marking a dropoff still work as expected.
    try await DoorstepAI.newEvent(
        eventName: "taking_pod",
        deliveryId: "some_delivery_id"
    )
}
```

When the delivery is complete + driver has exited the geofence, stop tracking by calling:

```swift
Task {
    await DoorstepAI.stopDelivery(deliveryId: "some_delivery_id")
}
```

#### Recommended Geofence Settings

- Use both an enter and exit geofence to bound the delivery session.
- A radius of 250m or larger typically yields the best results; 

### Step 5: Error Handling and Troubleshooting

- **Initialization Errors:** Check the Xcode console for any error messages during SDK setup.
- **Permission Denials:** Monitor the authorization status of location and motion services. Prompt the user for permission if needed.
- **Session Creation:** `startDelivery*` are `async throws`; handle failures (e.g., invalid API key) with `do/try/catch`.
---


