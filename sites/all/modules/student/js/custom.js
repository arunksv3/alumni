function withdraw(){
    var cnf = confirm('Are you sure you want to withdraw your application from this job?');
    if(cnf)
        return true;
    else
        return false;    
}

jQuery(document).ready(function($) {
    $('.preview_options').hide();
    $('.show').click(function () {        
        if($('#option' + $(this).attr('target')).is(':visible') == false){ 
            $('.preview_options').hide();
            $('#option' + $(this).attr('target')).show();
        }else{
            $('#option' + $(this).attr('target')).hide();
        }
    });
    
    $(".popup").click(function () {                    
        window.open($(this).attr("href"), "PopupWindow", "width=600,height=600,scrollbars=yes,resizable=no");
        return false;
    });
});