use <StepperMotor28BYJ_48.scad>

$fn = 128;

module wheelBase(spokeCount = 12)
{
    module spoke()
    {
        translate([0, -2, 0]) cube([38, 4, 7]); 
    }

    union()
    {
        difference()
        {
            cylinder(h = 7, r = 40);
            translate([0,0,-1]) cylinder(h = 10, r = 33);
        }

        for (az = [0 : spokeCount])
        { 
            rotate([0, 0, az*(360/spokeCount)]) spoke(); 
        }
        
        cylinder(h = 7, r = 8);
        cylinder(h = 12, r = 5.5);
    }
}

module wheel28BYJ_48(spokeCount = 12)
{
    difference()
    {
        wheelBase(spokeCount);
        translate([0,0,12])
            rotate([180,0,0])
                AxisCut_28BYJ_48();
      
    }   
}

wheel28BYJ_48(7);
