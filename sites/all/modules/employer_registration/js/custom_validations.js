jQuery(document).ready(function($) {
    $('#employer-registration-form, #employer-registration-profile-form').validate({ 
        rules: {            
            zipcode:{
                zipcode:true
            },
            email:{
                emailExp:true
            },             
            telephone:{
                numericExp:true,
                minlength:10,
                maxlength:14
            },
            fax:{
                numericExp:true,
                minlength:10,
                maxlength:14
            },
            'nature_bussiness[]':{
                required:true
            }
        },
        messages: {
            federal_title:{
                required: "Select your federal title"                
            },
            captcha_response:{
                required: "Enter above verification code"                
            },
            company_organization:{
                required: "Enter the name of Company / Organization"                
            }, 
            street:{
                required: "Enter your street"                
            }, 
            city:{
                required: "Enter your city"                
            },
            state:{
                required: "Select your state"                
            },
            country:{
                required: "Select your country"                
            },
            company_size:{
                required: "Enter your company size"                
            },
            website:{
                required: "Enter your website"                
            }, 
            intro_company_organisation:{
                required: "Enter brief introduction of your Company / Organization"    
            },
            p_title:{
                required: "Select your Title"    
            },
            firstname:{
                required: "Enter your First name"    
            },
            lastname:{
                required: "Enter your Last name"    
            },
            designation:{
                required: "Enter your Designation"   
            },
            email:{     
                required: "Enter your Email Address",   
                emailExp:"Please enter a valid email address"
            },             
            fax:{
                numericExp: "Only numbers are allowed",
                minlength: "Fax should have a minimum of 10 characters"
            },
            telephone:{    
                required: "Enter your Telephone",
                numericExp: "Only numbers are allowed",
                minlength: "Telephone should have a minimum of 10 characters"
            },
            zipcode:{
                required: "Enter your zip code", 
                zipcode: "Zip Code must be 5 numbers"
            },
            'nature_bussiness[]':{
                required:'You must check at least 1 box in Industry Type'
            }
        },        
        errorPlacement: function(error, element) {
            if (element.attr("name") == "nature_bussiness[]")
                error.insertAfter('#edit-nature-bussiness');                
            else
                error.insertAfter(element);    
        },
        debug:false        
    });    
    
    if($("#edit-nature-bussiness-64:checked").length == 1)
        $(".other-field").show();
    else
        $(".other-field").hide();
    
    $("#edit-nature-bussiness-64").click(function() {  
        if(this.checked == true)
            $(".other-field").show();
        else
            $(".other-field").hide();
    }); 
    
    $('#employer-registration-portfolio').validate();
});
