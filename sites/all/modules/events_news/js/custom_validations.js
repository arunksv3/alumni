jQuery(document).ready(function($) {
     
    $("#events-news-create-event").validate({
        rules: { 
            event_code:{required: true,},
            event_title:{required: true,} ,
            from_date:{required: true, },
            from_time:{required: true,},         
            to_date:{required: true,},            
            to_time:{required: true,},        
            location:{required: true,},            
            country:{required: true,},
            state:{required: true,},
            description:{required: true,},
            organized_by:{required: true,},
            note:{required: true,},
             
          }
    });
    
    $("#events-news-update-event").validate({
        rules: { 
            event_code:{required: true,},
            event_title:{required: true,} ,
            from_date:{required: true, },
            from_time:{required: true,},         
            to_date:{required: true,},            
            to_time:{required: true,},        
            location:{required: true,},            
            country:{required: true,},
            state:{required: true,},
            description:{required: true,},
            organized_by:{required: true,},
            note:{required: true,},
             
          }
    });
    
    
    $("#events-news-create-news").validate({
        rules: {  
            news_title:{required: true,} , 
            date:{required: true,}, 
            time:{required: true,},
            reported_by:{required: true,},
            category:{required: true,},
            description:{required: true,},
          }
    });
    
    $("#events-news-update-news").validate({
        rules: {  
            news_title:{required: true,} , 
            date:{required: true,}, 
            time:{required: true,},
            reported_by:{required: true,},
            category:{required: true,},
            description:{required: true,},
          }
    });

    /*$("#events-news-event").validate({
        rules: {  
            name:{required: true,} , 
            email:{required: true,email:true}, 
            //qualification:{required: true,},
            membership_type:{required: true,},
            mobile:{required: true,},
          }
    });*/
    
    
    
 }); 
function deleteImage(url,id){
    if(confirm('Delete Image?')){
        $('#img'+id).hide();
        $.ajax({
            type:"POST",
            url:url+"/"+id,
            success:function(data)
            {
               //alert(data);
            }
        }); 
     }
}