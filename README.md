# Compiling

Compile Drone (on raspberry pi): 

    make drone

Compile Client:

    make client
    
Testing socket:

    ./bin/test_server
    ./bin/test_client


Server output:

    Creating server on 0.0.0.0:8080
    Server started. Waiting for client data...
    Got Hello
    Sent back: ok
    Closing server socket...


Client output:

    Connecting client to 127.0.0.1:8080
    Client sending: Hello
    Message sent. Client waiting for response..
    Client received: ok

Testing UI:

    ./bin/client_ui

Sample output:

    Key pressed: Up
    New speed M1: 100 M2: 100 M3: 140 M4: 140
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: Down
    New speed M1: 140 M2: 140 M3: 100 M4: 100
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: Left
    New speed M1: 100 M2: 140 M3: 100 M4: 140
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: Right
    New speed M1: 140 M2: 100 M3: 140 M4: 100
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: w
    New speed M1: 150 M2: 150 M3: 150 M4: 150
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: s
    New speed M1: 88 M2: 88 M3: 88 M4: 88 100
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: a
    New speed M1: 104 M2: 120 M3: 120 M4: 104
    RESET M1: 120 M2: 120 M3: 120 M4: 120

    Key pressed: d
    New speed M1: 140 M2: 100 M3: 100 M4: 140
    RESET M1: 120 M2: 120 M3: 120 M4: 120