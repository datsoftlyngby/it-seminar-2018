$fn = 90;

module lineSensor() {
    function size(v) = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

    fs1 = [-3.5, 4.5, -1];
    fs2 = [3.5, 4.5, -1];

    led1 = [-3.5, 12.5, -2];
    led2 = [3.5, 12.5, -2];

    ch = 16;

    difference() {
        translate([-8, 0, 0]) cube([16, 16, ch]);
        
        translate(fs1) cylinder(h = 18, r = 2.5);
        translate(fs2) cylinder(h = 18, r = 2.5);
        
        translate(led1) rotate([20, 0, 0]) {
            cylinder(h = ch + 10, r = 2.5);
            cylinder(h = 3.5, r = 3.2);
            }
        translate(led2) rotate([20, 0, 0]) {
            cylinder(h = ch + 10, r = 2.5);
            cylinder(h = 3.5, r = 3.2);
            }
        
        }
    }

lineSensor();