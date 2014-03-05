import bb.cascades 1.2

Page {
    // create object at root level to access from CPP
    property alias tempText: destLabel.text

    Container {
        Label {
            id:header
            text:"Unofficial UWL Shuttle App beta version"  
            textStyle.color: Color.DarkCyan
            textStyle.fontWeight: FontWeight.Bold
            textFit.maxFontSizeValue: 50.0
            textFit.mode: LabelTextFitMode.Default
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        Label{
            id:destLabel
            text:"Which way are you going?"
        }
        
        DropDown {
            id:destMenu
            preferredWidth: 500
            onSelectedIndexChanged: {
                console.log("SelectedIndex was changed to " + selectedIndex);
            }
            
          Option {
              text:"Paragon"
              value: "ealing-paragon"
              
              onSelectedChanged: {
                  if (selected == true) {
                  locationLabel.visible = true
                  destinationEaling.visible=false
                  destinationParagon.visible=true
                  }
              }
          }
          
          Option {
              text:"Ealing Broadway"
              value:"paragon-ealing"
              onSelectedChanged: {
                  if (selected == true) {
                      locationLabel.visible = true
                      destinationParagon.visible=false
                     destinationEaling.visible = true
                  }
              }
          }
        }
        
        Label{
            id:locationLabel
            text:"Where are you now?"
            visible: false
        }
        
        DropDown {
            id:destinationParagon
            preferredWidth: 500
            visible:false
            Option {
                text:"Ealing Broadway Station"
                value: "ealing-broadway-station"             
            }

            Option {
                text:"High Street"
                value: "high-street"             
            }            
            
            Option {
                text:"St. Mary's Road"
                value: "st-marys"             
            }
            
            Option {
                text:"South Ealing Station"
                value: "south-ealing-station"             
            }
            
            Option {
                text:"Little Ealing Lane"
                value: "little-ealing-lane"             
            }
            onSelectedOptionChanged: {
                var currentDate = new Date();  
                var cur_hour = currentDate.getHours();
                var cur_min = currentDate.getMinutes();
                app.lookup_next_bus(destMenu.selectedValue, destinationParagon.selectedValue, cur_hour, cur_min);
                console.log(destMenu.selectedValue, destinationParagon.selectedValue);
                console.log(currentDate, cur_hour, cur_min);	
            }                        
        }
        
        DropDown {
            id:destinationEaling
            preferredWidth: 500
            visible: false
            Option {
                text:"Paragon"
                value: "paragon"             
            }
            Option {
                text:"Little Ealing Lane"
                value: "little-ealing-lane"             
            }
            Option {
                text:"South Ealing Station"
                value: "south-ealing-station"             
            }
            Option {
                text:"St. Mary's Road"
                value: "st-marys"             
            }
            Option {
                text:"Walpole House"
                value: "walpole-house"             
            } 
            onSelectedOptionChanged: {
                var currentDate = new Date();  
                var cur_hour = currentDate.getHours();
                var cur_min = currentDate.getMinutes();
                app.lookup_next_bus(destMenu.selectedValue, destinationEaling.selectedValue, cur_hour, cur_min);
                console.log(destMenu.selectedValue, destinationEaling.selectedValue);
                console.log(currentDate, cur_hour, cur_min);
            }   
        } 
        
        Label{
            id:nextBus
            text:app.status
        }

    }
}

