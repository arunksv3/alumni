jQuery(document).ready(function($) { 
     $("#employer-sendresumerequest").validate({
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
         contact_call:{
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
         contact_call:{
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
