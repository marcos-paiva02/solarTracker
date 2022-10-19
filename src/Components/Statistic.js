import React, { useState, useEffect, math } from 'react'
import firebase from './firebase'
import { ref, onValue} from 'firebase/database'

import '../css/Statistic.css'

let totalVoltage = 0
let vef = 0
let totalCurrent = 0
let ief = 0

export default function Statistic() {     

    const [voltageList, setVoltageList] = useState([])
    const [currentList, setCurrentList] = useState([])

    const voltageRef = ref(firebase, ('voltage'))
    const currentRef = ref(firebase, ('current'))
    
    useEffect(() => {
        onValue(voltageRef, (snapshot) => {
            const voltages = snapshot.val()
            const voltageList = []
            for (let id in voltages) {
                voltageList.push(voltages[id])
            }
            setVoltageList(voltageList)
            for (let i = 0; i < voltageList.length; i++) {
                totalVoltage = totalVoltage + parseInt(voltageList[i])
                vef = totalVoltage / voltageList.length
            }
        })

        onValue(currentRef, (snapshot) => {
            const currents = snapshot.val()
            const currentList = []
            for (let id in currents) {
                currentList.push(currents[id])
            }
            setCurrentList(currentList)
            for (let i = 0; i < currentList.length; i++) {
                totalCurrent = totalCurrent + parseInt(currentList[i])
                ief = totalCurrent / currentList.length
            }
        })
    }, [])

    return (
        <div className='statistic-container'>
            <div className='statistic-title'>Medições com rastreamento:</div>
            <div> <hr/> </div>
            <div className='statistic-data'>
                <div>V média: { vef.toFixed(1) } V</div>
                <div>I média: { ief.toFixed(1) }A </div>
                <div>P média: { (vef*ief).toFixed(1) }W </div>
            </div>           
        </div>
    )
}