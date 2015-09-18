jQuery(document).ready(function($) {
 
    $("#faculty-myaccount-form").validate({
        rules: {             
            //email:{required: true, email:true},
            dob:{required: true,},         
            mobile:{required: true,},            
            telephone:{required: true,},        
            nationality:{required: true,},            
            country:{required: true,},
            state:{required: true,},
            additional_info:{required: true,},
            department:{required: true,},
            physical_department:{required: true,},
          }       
    }); 
    
    $("#user-profile-form").validate({
        rules: {             
            //email:{required: true, email:true},
            dob:{required: true,},         
            mobile:{required: true,},            
            telephone:{required: true,},        
            nationality:{required: true,},            
            country:{required: true,},
            state:{required: true,},
            additional_info:{required: true,},
            department:{required: true,},
            physical_department:{required: true,},
          }       
    }); 
      
    $("#faculty-editaccount-form").validate({
        rules: {             
            //email:{required: true, email:true},
            dob:{required: true,},         
            mobile:{required: true,},            
            telephone:{required: true,},        
            nationality:{required: true,},            
            country:{required: true,},
            state:{required: true,},
            additional_info:{required: true,},
            department:{required: true,},
            physical_department:{required: true,},
          }       
    });
    
 }); 
