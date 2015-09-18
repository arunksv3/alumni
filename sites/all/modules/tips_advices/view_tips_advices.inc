<?php
function view_tips_advices($value){  
     global $user;
    $key = variable_get('IIUM_Encryption_Key', NULL);
    $cat_tips_advice = cat_tips_advice();
    $fields_of_study_arr = fields_of_study();
    $general_advice_list = general_advice_list();
    
    $query = db_select('tips_advices', 'tps');
    $query->fields('tps');    
    $query->condition('id', (int)decrypt($value, $key), '=');        
    $result = $query->execute()->fetchAssoc(); 
    
    //Image variables
    $image_path = array(
        'path' => $result['image_path'], 
        'alt' => $result['image_path'],
        'title' => $result['image_path'],  
        'width' => '150',
        'height' => '150', 
    );

    if($result['attachment']!=""){
        $attachArr = explode('/',$result['attachment']);
        $attchment_name = end($attachArr);
    }
    
    $output = '<div class="tp-view-header">
                    <h1>'.$result['title'].'</h1>  
              </div>             
              <div class="tp-view-content">
                        
                        <ul class="en-details-list">                           
                            <li><label>Category:</label> ';
                            if($result['category']=="0"){ 
                                $category =  $general_advice_list[$result['general_advice']];
                            }else if($result['category']=="1"){
                                $category =  getSpecializationParentName($result['career_p']).($result['career_sp']!="0"&&$result['career_sp']!=""?' >> '.getSpecializationSubparent($result['career_sp']):'').($result['career_c']!="0"&&$result['career_c']!=""?' >> '.getSpecializationChild($result['career_c']):'');
                            }else if($result['category']=="2"){
                                $category =  ($result['field_of_study']!="0"&&$result['field_of_study']!=""?$fields_of_study_arr[$result['field_of_study']]:'').($result['qualification']!="0"&&$result['qualification']!=""?' >> '.get_qualiafication_name($result['qualification']):'').($result['qualification_level']!="0"&&$result['qualification_level']!=""?' >> '.get_qualiafication_level_name($result['qualification_level']):'');
                            }else{
                                $category =  $nature_businesss_arr[$result['advice_industries']];
                            } 
                $output .= '<span>'.$cat_tips_advice[$result['category']].'</span>'.($category!=""&&$category!="0"? ' >> '.$category:'').'</li>
                            <li><label>Posted by:</label><span>'.$result['author_name'].'</span></li>
                        </ul>    
                        <div class="tp-content-img">'.
                           theme('image', $image_path).'
                        </div>                    
                        <p>'.$result['content_desc'].'</p>
                        <ul class="attachment">
                            '.($attchment_name!=""?'<li>Attachment: '.l(t('Download Attachment'), 'tips_advices/download/'.encrypt($attchment_name, $key)).'</li>':'').'
                            '.($result['embded_code']!=""?'<li class="video-sec">'.$result['embded_code'].'</li>':'').'
                        </ul>';
                        $path = current_path();
                        if(strpos($path,'admin')=== false)
                          $output .='<div class="bk-to-list"> <a href="javascript:history.go(-1)">Back to listing view</a> </div>';                            
        $output .= '</div>';
    
   return $output;
}
