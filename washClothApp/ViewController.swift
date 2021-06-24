//
//  ViewController.swift
//  washClothApp
//
//  Created by barrystone on 2021/6/19.
//

import UIKit

class ViewController: UIViewController {
    
    var washing = 0
    var startWashingTime : Date?
    
    @IBOutlet weak var doingText1: UILabel!
    
    @IBOutlet weak var doingText2: UILabel!
    
    @IBOutlet weak var doingTime: UILabel!
    
    @IBOutlet weak var finishText: UILabel!
    
    @IBOutlet weak var doingImage: UIImageView!
    
    @IBOutlet weak var finishImage: UIImageView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        //1325696 test
        //1339958 official
let url = URL(string: "https://api.thingspeak.com/channels/1339958/fields/1/last.json")!
        
        

        let task = URLSession.shared.dataTask(with: url) {(data, response, error) in
            guard let data = data else { return }
            let json = try?JSONSerialization.jsonObject(with: data, options: [])
            
            if let dictionary = json as? [String: Any] {
//                if let field1 = dictionary["field1"] as? String {
//                    updateImg(washing: field1);
//                }
                if let created_at = dictionary["created_at"] as? String {
                    let dateFormatter = DateFormatter()
                    dateFormatter.locale = .init(identifier: "en_US_POSIX")
                    dateFormatter.dateFormat = "yyyy-MM-dd'T'HH:mm:ssZ"
                    
//                    print(created_at);
                    self.startWashingTime = dateFormatter.date(from: created_at)
//                    print(self.startWashingTime)
                    washingDetected( startWashingTime : self.startWashingTime! )
                    updateImg()
                   
                }
            
            }

           
            
        }
 
        task.resume()
        
        func washingDetected(startWashingTime: Date){
                DispatchQueue.main.async{
               
                    let timeDiff = Int(2700 + floor(startWashingTime.timeIntervalSinceNow))
                    
                    if timeDiff < 0 {
                        self.washing = 0;
                        self.doingTime.text = "00:00"
                       
                    }else{
                        self.washing = 1;
                        self.doingTime.text = String(timeDiff / 60) + ":" + String(timeDiff % 60)
                    }
                
            }
        }
        
        func updateImg(){
                DispatchQueue.main.async{
               
                    if( self.washing == 1){
                   
                    self.doingText1.isHidden = false;
                    self.doingText2.isHidden = false;
                    self.doingTime.isHidden = false;
                    self.doingImage.isHidden = false;
                    
                    self.finishText.isHidden = true;
                    self.finishImage.isHidden = true;
                    

                }else{
                    self.doingText1.isHidden = true;
                    self.doingText2.isHidden = true;
                    self.doingTime.isHidden = true;
                    self.doingImage.isHidden = true;
                    
                    self.finishText.isHidden = false;
                    self.finishImage.isHidden = false;
                    
                }
                
            }
        }
        
        
        
    }
    
}

