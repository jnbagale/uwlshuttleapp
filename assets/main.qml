import bb.cascades 1.2

Page {
    // create object at root level to access from CPP
    property alias tempText: destLabel.text
    property int x_coord : 30
    property int y_coord : 30
    property int v_gap : 150
    
    Container {
        layout: AbsoluteLayout {
        }
        
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord
                positionY: y_coord
            }
            
            id:header
            text:"Unofficial UWL Shuttle App (beta)"  
            textStyle.color: Color.DarkCyan
            textStyle.fontWeight: FontWeight.Bold
            textFit.maxFontSizeValue: 50.0
            textFit.mode: LabelTextFitMode.Default
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        Label{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord
                positionY: y_coord + v_gap
            }
            
            id:destLabel
            text:"Towards"
            
        }
        
        DropDown {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord + 250
                positionY: y_coord + v_gap
            }
            id:destMenu
            preferredWidth: 400
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
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord
                positionY: y_coord + v_gap * 2
            }
            id:locationLabel
            text:"From"
            visible: false
        }
        
        DropDown {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord + 250
                positionY: y_coord + v_gap * 2
            }
            
            id:destinationParagon
            preferredWidth: 400
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
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord + 250
                positionY: y_coord + v_gap * 2
            }
            id:destinationEaling
            preferredWidth: 400
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
            layoutProperties: AbsoluteLayoutProperties {
                positionX: x_coord
                positionY: y_coord + v_gap * 5
            }
            id:nextBus
            text:app.status
        }

    }
}

