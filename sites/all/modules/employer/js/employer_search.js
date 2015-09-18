jQuery(document).ready(function($) { 

    
     // hide/show employment details
$('#edit-experience-level-0').click(function(){ 
  var lockChecked = jQuery('#edit-experience-level-0').attr('checked')?true:false;
     if(lockChecked == true)
      $( "#EmploymentDetails" ).hide();       
       
 }); 
 $('#edit-experience-level-1').click(function(){
  var lockChecked = jQuery('#edit-experience-level-1').attr('checked')?true:false;
     if(lockChecked == true)
      $( "#EmploymentDetails" ).hide();       
       
 });
 $('#edit-experience-level-2').click(function(){
  var lockChecked = jQuery('#edit-experience-level-2').attr('checked')?true:false;
     if(lockChecked == true)
      $( "#EmploymentDetails" ).show();       
       
 });
 
 

 
}); 
