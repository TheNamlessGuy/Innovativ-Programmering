;; 1 - Shakey
;; 2 - Slim door
;; 3 - Wide door
;; 4 - Lightswitch
;; 5 - Small object
;; 6 - Box
;;
;; +----------------------------------+
;; 4          |             |   55    |
;; |    6     3             2   55    |
;; |          |             |   55    |
;; |          |             |   55    4
;; |          4             |   55    |
;; |          |             |   55    |
;; |          2      1      3   55    |
;; |          |             |   55    |
;; +----------------------------------+
;;    room 1       room 2      room 3

;; Unique this time: 12 new toys, new goals

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
    toy1 toy2 toy3 toy4 toy5 toy6 toy7 toy8
    toy9 toy10 toy11 toy12 toy13 toy14 toy15 toy16

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
    (small-object toy2)
    (in room3 toy2)
    (small-object toy3)
    (in room3 toy3)
    (small-object toy4)
    (in room3 toy4)    
    (small-object toy5)
    (in room3 toy5)
    (small-object toy6)
    (in room3 toy6)
    (small-object toy7)
    (in room3 toy7)
    (small-object toy8)
    (in room3 toy8)
    (small-object toy9)
    (in room3 toy9)
    (small-object toy10)
    (in room3 toy10)
    (small-object toy11)
    (in room3 toy11)
    (small-object toy12)
    (in room3 toy12)
    (small-object toy13)
    (in room3 toy13)
    (small-object toy14)
    (in room3 toy14)
    (small-object toy15)
    (in room3 toy15)
    (small-object toy16)
    (in room3 toy16)

    ;; Shakey
    (shakey shakey1)
    (in room2 shakey1)
    (hand lhand)
    (hand rhand)
  )

  (:goal (and (in room1 toy1)
              (in room1 toy2)
              (in room1 toy3)
              (in room2 toy4)
              (in room1 toy5)
              (in room1 toy6)
              (in room2 toy7)
              (in room3 toy8)
              (in room3 toy9)
              (in room2 toy10)
              (in room2 toy11)
              (in room1 toy12)
              (in room1 toy13)
              (in room1 toy14)
              (in room1 toy15)
              (in room2 toy16)))
)