$fn = 90;

module wheel(spokeCount = 12) {

    module spoke() {
        translate([0, -2, 0]) cube([38, 4, 7]); 
        }

    union() {
        difference() {
            cylinder(h = 7, r = 40);
            translate([0,0,-1]) cylinder(h = 10, r = 33);
            }

        for (az = [0 : spokeCount]) { 
            rotate([0, 0, az*(360/spokeCount)]) spoke(); 
            }

        difference() {
            cylinder(h = 20, r = 10);
            translate([-1.5, -2.5, 1]) cube([3, 5, 12]);
            }
        }
    }
    
translate([-50, 0, 0]) wheel(7);
translate([50, 0, 0]) wheel(7);
