$fn = 90;

function size(v) = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

//fs1 = [-0.35, 0.45, -0.1];
fs1 = [-0.35, 0.45, -0.1];
fs2 = [0.35, 0.45, -0.1];

led1 = [-0.35, 1.25, -0.2];
led2 = [0.35, 1.25, -0.2];

ch = 1.6;

difference() {
    translate([-0.8, 0, 0]) cube([1.6, 1.6, ch]);
    
    translate(fs1) cylinder(h = 1.8, r = 0.25);
    translate(fs2) cylinder(h = 1.8, r = 0.25);
    
    translate(led1) rotate([20, 0, 0]) {
        cylinder(h = ch + 1, r = 0.25);
        cylinder(h = 0.35, r = 0.32);
        }
    translate(led2) rotate([20, 0, 0]) {
        cylinder(h = ch + 1, r = 0.25);
        cylinder(h = 0.35, r = 0.32);
        }
    
    //rotate([0, 0, 30])
        
    }

