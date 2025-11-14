# DoorstepDropoffSDK Documentation

---

## iOS Integration

### Prerequisites

- **Development Environment:** Xcode (version 12 or later is recommended) with a target of iOS 14 or higher.
- **API Key:** A valid API key provided by DoorstepAI.

### Step 1: Add the SDK Dependency

Integrate the SDK using Swift Package Manager and import (check example application for package source):

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

Set the API Key in your main view controller or app delegate

```swift
import DoorstepDropoffSDK

struct MyApp: App {
    init() {
        DooorstepAI.setApiKey("YOUR_API_KEY")
    }
    
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
```

Initialize the Root Component within the ContentView

```swift

struct ContentView: View {
    var body: some View {
        DoorstepAIRoot()
        // ...
    }
}
```

### Step 4: Starting and Stopping Delivery Tracking

Begin tracking by calling one of the provided methods:

```swift
// Start delivery using a Google PlaceID
DoorstepAI.startDeliveryByPlaceID(placeID: "placeID_here", deliveryId: "some_delivery_id")

// Start delivery using a Google PlusCode
DoorstepAI.startDeliveryByPlusCode(plusCode: "plusCode_here", deliveryId: "some_delivery_id")

// Start delivery using Address Components
let address = AddressType(
    streetNumber: "123",
    route: "Main St",
    subPremise: "Unit A",
    locality: "City",
    administrativeAreaLevel1: "State",
    postalCode: "12345"
)
DoorstepAI.startDeliveryByAddressType(address: address, deliveryId: "some_delivery_id")

// start delivery using a single address string
let address = "123 Main St, Unit A, City, State, 12345"
DoorstepAI.startDeliveryByAddressString(address: address, deliveryId: "some_delivery_id")
```

Record a dropoff with the following event recording methods:

```swift
// use this event trigger when activating a POD capture
DoorstepAI.newEvent("taking_pod", "some_delivey_id")

// use this event trigger when POD capture is complete
DoorstepAI.newEvent("taking_pod", "pod_captured")

// use this event trigger there is no POD, only a dropoff
DoorstepAI.newEvent("taking_pod", "dropoff")

```

When the delivery is complete, stop tracking by calling:

```swift
DoorstepAI.stopDelivery(deliveryId: "some_delivery_id")
```
### Step 5: Error Handling and Troubleshooting

- **Initialization Errors:** Check the Xcode console for any error messages during SDK setup.
- **Permission Denials:** Monitor the authorization status of location and motion services. Prompt the user for permission if needed.

---


