$fn = 90;

module wheel(spokeCount = 12) {

    module spoke() {
        translate([0, -0.2, 0]) cube([3.8, 0.4, 0.7]); 
        }

    union() {
        difference() {
            cylinder(h = 0.7, r = 4);
            translate([0,0,-0.1]) cylinder(h = 1, r = 3.3);
            }

        for (az = [0 : spokeCount]) { 
            rotate([0, 0, az*(360/spokeCount)]) spoke(); 
            }

        difference() {
            cylinder(h = 2, r = 1);
            translate([-0.15, -0.25, 1]) cube([0.3, 0.5, 1.2]);
            }
        }
    }
    
translate([-5, 0, 0]) wheel(9);
translate([5, 0, 0]) wheel(7);
