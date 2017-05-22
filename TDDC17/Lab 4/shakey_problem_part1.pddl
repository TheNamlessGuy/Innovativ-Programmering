;; 1 - Shakey
;; 2 - Slim door
;; 3 - Wide door
;; 4 - Lightswitch
;; 5 - Small object
;; 6 - Box
;;
;; +----------------------------------+
;; 4          |             |         |
;; |    6     3             2         |
;; |          |             |         |
;; |          |             |         4
;; |          4             |         |
;; |          |             |         |
;; |          2      1      3    5    |
;; |          |             |         |
;; +----------------------------------+
;;    room 1       room 2      room 3

(define (problem shakey-1)
  (:domain shakey)
  (:objects
    ;; Rooms
    room1 room2 room3

    ;; Doors
    wide1 wide2 slim1 slim2

    ;; Lightswitches
    switch1 switch2 switch3

    ;; Boxes
    box1

    ;; Small objects
    toy1

    ;; Shakey
    shakey1 lhand rhand
  )

  (:init
    ;; Rooms
    (room room1) (room room2) (room room3)

    ;; Doors
    (door wide1) (door wide2) (door slim1) (door slim2)
    (is-wide wide1) (is-wide wide2)
    (connected room1 room2 wide1) (connected room1 room2 slim1) (connected room2 room3 wide2) (connected room2 room3 slim2)
    (connected room2 room1 wide1) (connected room2 room1 slim1) (connected room3 room2 wide2) (connected room3 room2 slim2)

    ;; Lightswitches
    (lightswitch switch1) (lightswitch switch2) (lightswitch switch3)
    (in room1 switch1) (in room2 switch2) (in room3 switch3)
    
    ;; Boxes
    (box box1)
    (in room1 box1)

    ;; Small objects
    (small-object toy1)
    (in room3 toy1)

    ;; Shakey
    (shakey shakey1)
    (in room2 shakey1)
    (hand lhand)
    (hand rhand)
  )

  (:goal (in room1 toy1))
)