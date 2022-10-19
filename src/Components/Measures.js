import React, { useState, useEffect, Text } from 'react'
import firebase from './firebase'
import { ref, onValue} from 'firebase/database'
import { Chart } from "react-google-charts";

export const options = {
    title: "Eficiência com rastreador solar",
    hAxis: { title: "Eficiência", titleTextStyle: { color: "#333" } },
    vAxis: { minValue: 0 },
    chartArea: { width: "50%", height: "70%" },
  };

let data = []

export default function CurrentList() {     

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
                data[i + 1] = [i + 1, parseInt(voltageList[i]), 0]
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
                data[i + 1][2] = parseInt(currentList[i])
            }
        })
        data[0] = ['dia', 'tensão', 'corrente']
    }, [])

    return (
        <div className='measures-container'>
            <Chart
                chartType="AreaChart"
                width="100%"
                height="100%"
                data={data}
                options={options}
            />
        </div>
    )
}
