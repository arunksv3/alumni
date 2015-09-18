jQuery(document).ready(function($) {
         
    $('#studentDiv').click(function(){
         $('#experience').hide();
         $('#student').show();
         $( "#studentDiv" ).addClass( "details_active" );
         $( "#experienceDiv" ).removeClass( "details_active" );
    });
    
    $('#experienceDiv').click(function(){
         $('#student').hide();
         $('#experience').show();
         $( "#experienceDiv" ).addClass( "details_active" );
         $( "#studentDiv" ).removeClass( "details_active" );
    });
    
    $('#qualificationDiv').click(function(){          
         $('#secondHighestDiv').show();
    });
    
    $('#hideQualificationDiv').click(function(){          
         $('#secondHighestDiv').hide();
    });
    
    $('#EditAdditionalInfo').click(function(){          
         $('#additionalInfo').hide();
         $('#additionalInfoDiv').show();
         $('#EditAdditionalInfo').hide();
    });
    $('#CancelAdditionalInfo').click(function(){          
         $('#additionalInfoDiv').hide();
         $('#additionalInfo').show();
         $('#EditAdditionalInfo').hide();
    });
    
    
    
    $('#EditSkills').click(function(){
         $('#SkillsDiv').hide();
         $('#EditSkillsDiv').show();
    });
    $('#CancelSkills').click(function(){          
         $('#EditSkillsDiv').hide();
         $('#SkillsDiv').show();
    });
    
    $('#EditLanguages').click(function(){
         $('#LanguagesDiv').hide();
         $('#EditLanguagesDiv').show();
    });
    $('#CancelLanguages').click(function(){          
         $('#EditLanguagesDiv').hide();
         $('#LanguagesDiv').show();
    });
    
    
    $('#editEducationDiv1').click(function(){
         $('input[name="edu_type"]').val('2');
         $('#HighestDiv1').show();
         $('#buttonsDiv').show();
         $('#HighestDiv2').hide();
         $('#Education1').hide();
         $('#Education2').hide();
         $('#addEducation').hide();
         $('#stdresult').hide();
         $('#stdprogram1').hide();
          $('#stdprogram2').hide();
         $('#kulyname').hide();
    });
    
     $('#editEducationDiv2').click(function(){
         $('input[name="edu_type"]').val('1');
         $('#HighestDiv2').show();
         $('#buttonsDiv').show();
         $('#HighestDiv1').hide();
         $('#Education1').hide();
         $('#Education2').hide();
         $('#stdresult').hide();
          $('#stdprogram1').hide();
         $('#stdprogram2').hide();
         $('#kulyname').hide();
    });
    
    $('#hideEducationDiv1').click(function(){          
        $('#HighestDiv1').hide();
        $('#HighestDiv2').hide();
        $('#buttonsDiv').hide();
        $('#Education1').show();
        $('#Education2').show();
    });
    
    
  
  
  
  
   $('#addExperienceDiv').click(function(){   
         $('#Edit_Experience_Div').show();
         $('#buttonsDiv').show(); 
         $('#ExperienceDiv1').hide(); 
         $('#viewExperienceDiv').hide();
         $('#ExperienceLevelEdit').hide();
    });
    
     $('#editExperienceDiv').click(function(){
         $('#ExperienceDiv1').show();
         $('#buttonsDiv').show();
         $('#Edit_Experience_Div').hide();
         $('#viewExperienceDiv').hide();
          
    });
    
    $('#hideExperienceDiv').click(function(){          
        $('#Edit_Experience_Div').hide();
        $('#ExperienceDiv1').hide();
        $('#buttonsDiv').hide();
        $('#viewExperienceDiv').show();
    });
    
//    $('#hideEducationDiv2').click(function(){          
//         $('#HighestDiv2').hide();
//    }); 
    
    
   $('#EditAboutMe').click(function(){          
         $('#aboutme').hide();
         $('#aboutmeDiv').show();
    });
    $('#CancelAboutme').click(function(){          
         $('#aboutmeDiv').hide();
         $('#aboutme').show();
    });   
    
    
    $('#edit-level-0').click(function(){          
         $('#ExperienceDiv').show();
         $('#WorkingDate').show();
    });
    
    $('#edit-level-1').click(function(){          
         $('#ExperienceDiv').hide();
         $('#WorkingDate').hide();
     });  
    
    $('#edit-level-2').click(function(){          
         $('#ExperienceDiv').hide();
         $('#WorkingDate').hide();
     });
     
     
     $('#edit-resume1-1').click(function(){          
         $('#privacyDiv').show();
    });
    
    $('#edit-resume1-0').click(function(){          
         $('#privacyDiv').hide();
     });
     
     
     
     $("#student-details-form").validate({
      rules: {
 /*
         firstname:{
		required: true,
                alphaExp:true
            },
         lastname:{
		required: true,
                alphaExp:true
            } ,
         state:{
		required: true,
            },
         country:{
		required: true,
            },
         
        telephone:{
		required: true,
                phoneexp:true
            },
            
        mobile:{
		required: true,
                phoneexp:true
            },
        
        email:{
		required: true,
                emailExp:true
            },
        dob:{
		required: true,
             },
        
        additional_info:{
		required: true,
             },
        nationality:{
            required: true,
         }, */
         country:{
		required: true,
             },
        state:{
           required: true,
        },
        highest_qualification:{
		required: true,
             }, 
        highest_qualification_level:{
		required: true,
             }, 
        field_of_study:{
		required: true,
             }, 
        specializationp:{
		required: true,
             }, 
       specializationsp:{
		required: true,
             },
        specializationc:{
		required: true,
             },
        expected_salary:{
		required: true,
             },
        location_country:{
		required: true,
             },
        location_state:{
		required: true,
             },  
        expected_salary1:{
		required: true,
             },
        experience_in_months:{
		required: true,
             },
        experience_in_years:{
		required: true,
             },
        location_country1:{
		required: true,
             },
        location_state1:{
		required: true,
             },
        specializationp1:{
		required: true,
             },
        specializationsp1:{
		required: true,
             },
        specializationc1:{
		required: true,
             },  
        state1:{
		required: true,
            },
         country1:{
		required: true,
            },
         
      },
     messages: {
            firstname:{
              required: "Enter Firstname",
              alphaExp:"Letters and numbers only. No special characters"
            },
            lastname:{
              required: "Enter Lastname",
              alphaExp:"Letters and numbers only. No special characters"
            },
            email:{
              required: "Enter Email",
              emailExp:"Invalid Email Address"
            },
            mobile:{
		required: "Enter Mobile No.",
                phoneexp:"Invalid Mobile No."
            },
      } 
       
   }); 
   
  $("#student-edit-experience-form").validate({
         rules: { 
             company_name1:{required: true,},
             industry1:{required: true,} ,
             date_joined1:{required: true, },
             date_left1:{required: true,},         
             job_title1:{required: true,},            
             position1:{required: true,},        
             specializationp1:{required: true,},  
             specializationsp1:{required: true,},  
             specializationc1:{required: true,},  
             company_name:{required: true,},
             industry:{required: true,} ,
             date_joined:{required: true, },
             date_left:{required: true,},         
             job_title:{required: true,},            
             position:{required: true,},        
             specializationp:{required: true,},
             specializationsp:{required: true,},  
             specializationc:{required: true,},  
             location_country:{required: true,},
             location_state:{required: true,},
             location_country1:{required: true,},
             location_state1:{required: true,},
             salary:{required: true,},
             salary1:{required: true,},
           }       
});

$("#student-create-resume-form").validate({
         rules: { 
           /*  firstname:{required: true,},
             lastname:{required: true,} ,
             email:{required: true, },
             dob:{required: true,},         
             mobile:{required: true,},            
             telephone:{required: true,},    */    
             nationality:{required: true,},            
             country:{required: true,},
           }       
});

$("#student-update-resume-form").validate({
    rules: { 
        resume:{required: true,},
    }       
});

$("#student-upload-resume-form").validate({
    rules: { 
        
        company_name:{required: true,},
        industry:{required: true,},
        date_joined:{required: true,},
        date_left:{required: true,},
        job_title:{required: true,},
        position:{required: true,},
        specializationp:{required: true,},
        specializations:{required: true,},
        specializationc:{required: true,}, 
        location_country:{required: true,},
        location_state:{required: true,},
        salary:{required: true,},
        
        qualification:{required: true,},
        qualificationlevel:{required: true,},
        fieldofstudy:{required: true,},
        university:{required: true,},
        graduationdate:{required: true,},  
        country:{required: true,},
        state:{required: true,}, 
        
        qualification1:{required: true,},
        qualification_level1:{required: true,},
        field_of_study1:{required: true,},
        university1:{required: true,},
        graduation_date1:{required: true,}, 
        country1:{required: true,},
        state1:{required: true,},
        
    }       
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
 
 
 
 $("#student-edit-experience-form").validate({
    rules: { 
        company_name1:{required: true,},
        industry1:{required: true,} ,
        date_joined1:{required: true, },
        date_left1:{required: true,},         
        job_title1:{required: true,},            
        position1:{required: true,},        
        specializationp1:{required: true,},
        specializationsp1:{required: true,},
        specializationc1:{required: true,},
        company_name:{required: true,},
        industry:{required: true,} ,
        date_joined:{required: true, },
        date_left:{required: true,},         
        job_title:{required: true,},            
        position:{required: true,},        
        specializationp:{required: true,}, 
        specializationsp:{required: true,}, 
        specializationc:{required: true,} 
    }       
});


$("#student-edit-education-form").validate({
    rules: { 
        qualification:{required: true,},
        qualification_level:{required: true,} ,
        field_of_study:{required: true, },
        university:{required: true,},         
        country:{required: true,},   
        state:{required: true,},   
        graduation_date:{required: true,},        
        qualification1:{required: true,},
        qualification_level1:{required: true,} ,
        field_of_study1:{required: true, },
        university1:{required: true,},         
        country1:{required: true,},   
        state1:{required: true,},   
        graduation_date1:{required: true,}, 
    }       
});


$("#student-edit-skills-form").validate({
    rules: { 
        "skills[]":{required: true,},
        "proficiency[]":{required: true,} , 
    }       
});

$("#student-edit-languages-form").validate({
    rules: { 
        "language[]":{required: true,},
        "spoken[]":{required: true,} , 
        "written[]":{required: true,} , 
        //"certificate[]":{required: true,} , 
        //"primary[]":{required: true,} , 
    }       
});

    $("#student-edit-additional-info-form").validate({
        rules: { 
            "expected_salary":{required: true,},
            "preferred_country":{required: true,} , 
            "preferred_state":{required: true,} , 
        }       
    });

    $("#student-aboutme-form").validate({
        rules: { 
//            "firstname":{required: true,},
//            "lastname":{required: true,} , 
//            "dob":{required: true,} , 
            "country":{required: true,} , 
//            "mobile":{required: true,} , 
//            "telephone":{required: true,} , 
//            "email":{required: true,} , 
        }       
    });
    
    
    
    $('#ExperienceLevelEdit').click(function(){          
        $('#editExperienceLevel').show();
        $('#addExperienceDiv').hide();
        $('#ExperienceLevelEdit').hide();
        $('#edit_experience').hide();
     });  
    

    $("#student-add-education-form").validate({
        rules: { 
            qualification:{required: true,},
            qualification_level:{required: true,} ,
            field_of_study:{required: true, },
            university:{required: true,},         
            country:{required: true,},            
            state:{required: true,},            
            graduation_date:{required: true,},        
         
        }       
    });



    $("#present").click(function () { 
        if($(this).is(":checked")){
            $("#date_left").attr("disabled", "disabled"); 
            $("#date_left").val('');
         }
    	else
            $("#date_left").removeAttr("disabled"); 
      });
      
      $("#present1").click(function () { 
        if($(this).is(":checked")){
            $("#date_left1").attr("disabled", "disabled"); 
            $("#date_left1").val('');
         }
    	else
            $("#date_left1").removeAttr("disabled"); 
      });
      
 }); 
