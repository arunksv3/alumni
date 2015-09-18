jQuery(document).ready(function($) { 
     $("#employer-interviewrequest").validate({
      rules: {
          email_id:{
		required: true,
	        emailExp:true
            },
          subject:{
		required: true,
                alphaExp:false
            },
          message_all:{
		required: true
            }, 
          from:{
		required: true
            }, 
          to:{
		required: true
            },
          street:{
                required: true               
            }, 
          city:{
                required: true ,
                alphaExp:true
            },
          state:{
                required: true,
                 alphaExp:true
            },
          country:{
                required: true,
                alphaExp:true
            },
          zip_code:{
		required: true,
                numericExp:true
            },
         fname:{
		required: true,
                alphaExp:true
            },
         lname:{
		required: true,
                alphaExp:true
            },
         designation:{
		required: true,
                alphaExp:true
            },
         company:{
		required: true
            },
         call:{
		phoneexp: true
            },
         email:{
		emailExp: true
            },
      },
     messages: {
            email_id:{
              required: "Enter Your Email ID ",
              emailExp:"Invalid email address"
            }, 
            subject:{
              required: "Enter Subject/Job Title ",
              alphaExp:"Letters and numbers only. No special characters"
            },
            message_all:{
		required: "Enter Message",
            }, 
           from:{
		required: "Enter From Date",
            }, 
           to:{
		required: "Enter To Date",
            },
           street:{
                required: "Enter your street"           
            }, 
           city:{
               required: "Enter city",
               alphaExp:"Letters and numbers only. No special characters"   
            },
           state:{
                required: "Select your state",
                alphaExp:"Letters and numbers only. No special characters"
            },
           country:{
                required: "Select your country",
                alphaExp:"Letters and numbers only. No special characters"
            },
          zip_code:{
		required: "Enter Zip code",
                numericExp:"Invalid zipcode"
            },
          fname:{
		required: "Enter Firstname",
                alphaExp:"Letters and numbers only. No special characters"
            },
         lname:{
		required: "Enter Lastname",
                alphaExp:"Letters and numbers only. No special characters"
            },
         designation:{
		required: "Enter Designation",
                alphaExp:"Letters and numbers only. No special characters"
            },
         company:{
		required: "Enter Company",
            },
         call:{
		 phoneexp:"Invalid phone number"

            },
         email:{
		 emailExp:"Invalid email address"
            
            },
      },
   errorPlacement: function(error, element) {
       error.insertAfter(element);    
     },
      debug:false,
   }); 
   
 }); 
