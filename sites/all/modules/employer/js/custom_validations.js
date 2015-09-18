jQuery(document).ready(function($) { 
     $("#employer-form").validate({
      rules: {
          jobtitle:{
		required: true,
	        alphaExp:false
            },
          positionoffered:{
		required: true
            },
          numberofvacancies:{
		required: true,
                numericExp:true
            },
         'Jobsfor_checkboxes[]':{
		required: true          
            },
         'categories_checkboxes[]':{
		required: true          
            },
          categories:{
		required: true
            },
          Jobdescription:{
		required: true
            },
          requirments:{
	      required: true
            },
          fieldsofstudy:{
		required: true
            },
          'level_of_awards[]':{
		required: true
            },
          job_country:{
		required: true
            },
          job_country:{
		required: true
            },
          job_state:{
		required: true
            },
          salary:{
		required: true,
                salaryexp:true
            },
          languageskills:{
		required: false,
                alphaExp:false
            }, 
          otherskills:{
		required: false,
                alphaExp:false
            },
          applicable_call:{
		required: false,
                numericExp:true
            },
          applicable_email:{
		required: false,
                emailExp:true
            },
          applicable_others:{
		required: false,
                alphaExp:false
            },
          deadline:{
		required: true
            }, 
          close_jobpost:{
		required: true
            },
         title:{
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
         addressline_1:{
		required: true
            },
         city:{
		required: true,
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
        telephone:{
		required: true,
                phoneexp:true
            },
        fax:{
		required: false,
                faxexp:true
            },
        email_valid:{
		required: true,
                emailExp:true
            },
      
      },
     messages: {
            jobtitle:{
              required: "Enter Job Title",
              alphaExp:"Letters and numbers only. No special characters"
            }, 
            positionoffered:{
              required: "Select Position Offered",
            },
            numberofvacancies:{
	      required: "Enter Number of Vacancies",
              numericExp:"Numbers only. No Letters and special characters"
            },
           'Jobsfor_checkboxes[]':{
		required: "You must check at least 1 box in Job Type",              
            },
           'categories_checkboxes[]':{
		required: "You must check at least 1 box in Categories",              
            },
           categories:{
	      required: "Select Categories"
            },
           Jobdescription:{
	      required: "Enter Job Description"
            },
           requirments:{
	      required: "Enter Requirments"
            },
           fieldsofstudy:{
		required: "Select Fields of Study Required"
            }, 
           'level_of_awards[]':{
		required: "You must check at least 1 box in Level of Awards", 
            },
           job_country:{
		required: "Select Country"
            }, 
           job_state:{
		required: "Select State"
            }, 
           year_of_experience:{
		required: "Select Minimum Experience Required"
            }, 
           salary:{
		required: "Enter salary (MYR) in digits",
                salaryexp:"Numbers only. No Letters and special characters"
            }, 
           languageskills:{
		required: "Enter Lanaguage Skills",
                alphaExp:"Letters and numbers only. No special characters"
            }, 
           otherskills:{
		required: "Enter Other Skills",
                alphaExp:"Letters and numbers only. No special characters"
            }, 
           applicable_call:{
                numericExp:"Numbers only. No Letters and special characters"
            },
           applicable_email:{
                emailExp:"Invalid email address"
            },
           applicable_others:{
                 alphaExp:"Letters and numbers only. No special characters"
            },
          deadline:{
		required: "Select Deadline for Application"
            },
          close_jobpost:{
		required: "Select Close of this Job Post Date"
            }, 
         
          title:{
		required: "Select Title"
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
         addressline_1:{
		required: "Enter Address line 1",
            },
         city:{
		required: "Enter City",
                alphaExp:"Letters and numbers only. No special characters"
            },
         state:{
		required: "Enter State",
                alphaExp:"Letters and numbers only. No special characters"
            },
         country:{
		required: "Enter Country",
                alphaExp:"Letters and numbers only. No special characters"
            },
         zip_code:{
		required: "Enter Zip code",
                numericExp:"Invalid zipcode"
            },
         telephone:{
		required: "Enter Telephone",
                phoneexp:"Invalid phone number"
            },
         fax:{
		required: "Enter Fax",
                faxexp:"Invalid fax number"
            },
         email_valid:{
		required: "Enter Email",
                emailExp:"Invalid email address"
            },
       
      },
   errorPlacement: function(error, element) {
      if (element.attr("name") == "Jobsfor_checkboxes[]")
       error.insertAfter('#Jobsfor');
      else if (element.attr("name") == "categories_checkboxes[]")
       error.insertAfter('#categories');
      else if (element.attr("name") == "level_of_awards[]")
       error.insertAfter('#level_of_awards_input');
      else
       error.insertAfter(element);    
     },
      debug:false,
   }); 
   
$('#information_provided').click(function(){
  var lockChecked = jQuery('#information_provided').attr('checked')?true:false;
     if(lockChecked == true)
      $( "#contact_person_information_input" ).show();           
     else
      $( "#contact_person_information_input" ).hide();      
 });
$('#edit-jobsfor-checkboxes-5').click(function(){
  var lockChecked = jQuery('#edit-jobsfor-checkboxes-5').attr('checked')?true:false;
     if(lockChecked == true)
      $( "#outher_inputtextfield" ).show();           
     else
      $( "#outher_inputtextfield" ).hide();      
 });  

 }); 
